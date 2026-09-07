# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

Dreamscape — Unreal Engine **5.4** C++ 게임 프로젝트. 아이소메트릭 탑뷰 액션.

- 엔진 설치 경로: `C:\UnrealEngine\UE_5.4` (`Dreamscape.uproject`의 EngineAssociation = "5.4")
- 게임 모듈: `Source/Dreamscape` (단일 모듈). 의존성은 `Dreamscape.Build.cs` — EnhancedInput, UMG, Niagara, GameplayTags 등
- 클래스 접두사는 `DS` (`ADSCharacterPlayer`, `UDSInventoryComponent` …)
- **주석과 기획 문서가 한국어다.** 코드를 수정할 때 주변 주석의 언어를 따를 것
- 상위 기획서는 사용자의 Notion `Dreamscape/문서/시스템기획`에 있다. 시점·이동 규칙은 그쪽이 정본

## 빌드

에디터 타깃 빌드 (테스트 스위트·린터는 이 프로젝트에 없다):

```bash
"C:/UnrealEngine/UE_5.4/Engine/Build/BatchFiles/Build.bat" DreamscapeEditor Win64 Development -Project="C:/Users/User/Documents/Unreal Projects/Dreamscape/Dreamscape/Dreamscape.uproject" -WaitMutex
```

- 증분 빌드는 10초 내외로 끝난다
- **에디터가 켜져 있으면 DLL 링크에 실패한다.** 빌드 전에 `tasklist | grep -i unreal` 로 확인할 것
- 게임플레이 검증(카메라 프레이밍, 조작 방향, FSM 전이)은 PIE에서만 가능하다. 빌드 통과 ≠ 동작 확인이므로 무엇을 검증하지 못했는지 명시할 것

## 소스 파일 규약

`.h` / `.cpp` 는 전부 **UTF-8 with BOM + CRLF** 다. 스크립트로 편집할 때 둘 다 보존해야 한다.
`sed -i` 는 치환한 줄의 CR을 떨어뜨려 git이 line-ending 경고를 낸다. Python으로 다루는 쪽이 안전하다:

```python
src = io.open(path, 'r', encoding='utf-8-sig', newline=None).read()
io.open(path, 'w', encoding='utf-8-sig', newline='\r\n').write(src)
```

## 아키텍처

### 1. 데이터 주도 캐릭터 컨트롤 (`UDSCharacterControlData`)

카메라·이동·입력 매핑의 **런타임 실제 값은 C++ 생성자가 아니라 데이터 에셋에 있다.**

`ADSCharacterBase` 생성자가 `/Game/Character/DSC_Move` 를 `CharacterControlManager[ECharacterControlType::Quarter]` 에 넣고,
`ADSCharacterPlayer::BeginPlay` 의 `SetCharacterControl(Quarter)` 가 `SetCharacterControlData()` 를 통해
스프링암·카메라·무브먼트 설정을 **전부 덮어쓴다**. 생성자 값은 사실상 기본값일 뿐이다.

**중요한 함정**: UE는 CDO 기본값과 같은 프로퍼티를 `.uasset`에 직렬화하지 않는다.
따라서 데이터 에셋에서 "건드리지 않은" 값은 C++ 생성자 기본값을 그대로 따라간다.
C++ 기본값을 바꾸면 에셋을 수정하지 않아도 게임 동작이 조용히 바뀐다는 뜻이다.
(실제로 `FOV`가 이 상태였고, 기획서의 4~10 대신 90으로 돌고 있었다.)

`DSC_Move`가 실제로 직렬화하고 있는 값: `TargetArmLength=12500`, `RelativeRotation=(-45, 0, 0)`,
`RotationRate=(0, 720, 0)`, `MaxWalkSpeed=275`, `InputMappingContext=IMC_Default`.
나머지(`bInheritPitch/Yaw/Roll`, `bUsePawnControlRotation`, `bDoCollisionTest`, `bOrientRotationToMovement` …)는 전부 CDO 기본값 = false.

### 2. 입력 → FSM → 캐릭터 (직접 호출 금지)

플레이어 입력은 항상 이 경로를 탄다:

```
EnhancedInput → ADSCharacterPlayer::HandleXxx()
              → UDSPlayerFSMComponent::HandleXxxInput()
              → CurrentState->OnXxx()
              → ADSCharacterPlayer::Xxx()   // 실제 동작
```

입력 핸들러에서 캐릭터 동작 함수를 바로 부르면 상태 전이를 건너뛴다. 새 액션을 추가할 때도 이 경로를 유지할 것.

`UDSPlayerFSMComponent`는 **푸시다운 오토마타**다 — `ChangeState()`는 교체, `PushPlayerState()`/`PopPlayerState()`는
현재 상태를 스택에 남긴 채 임시 상태(구르기·피격 등)를 얹는다. 상태는 `Components/PlayerStateBase/` 의 UObject들이고
`UDSPlayerStateBase`의 `Enter/Update/Exit` + `OnMove/OnRoll/OnFall/...` 를 오버라이드한다.

주의: `UDSPlayerState_Idle::OnMove`는 Walk로 전이만 하고 그 프레임에는 이동하지 않는다.

### 3. 카메라와 이동 규약 (기획 핵심)

- **+X가 화면 위쪽이자 플레이어 정면.** 45도 아이소메트릭 룩은 카메라를 돌려서가 아니라
  **맵(레벨 인스턴스/서브레벨 부모 트랜스폼)을 45도 회전시켜** 만든다. 카메라 Yaw는 0이 기본
- **FOV 4~10** (현재 8) + 매우 긴 암 길이. 화면 가로 폭 `ViewWidth = 2 * ArmLength * tan(FOV/2)`,
  역산은 `ArmLength = (ViewWidth/2) / tan(FOV/2)`. 거의 직교 투영에 가까워 깊이가 다른 물체가 화면에서 겹쳐 보인다
- 스프링암은 `bInherit*` 전부 false → **액터 회전과 무관하게 월드 고정**. 캐릭터가 돌아도 카메라는 안 돈다
- 암 길이가 12500이라 `bDoCollisionTest`는 false, 회전 랙도 false(1도 지연이 약 218유닛 위치 스윙이 된다)

**이동 방향은 반드시 `ADSCharacterPlayer::GetCameraYaw()` 를 기준으로 계산한다.**
`Controller->GetControlRotation()`을 쓰면 안 된다 — 카메라 붐과 아무 연결이 없고, `IA_Look`이 바인딩되지 않은
현재 구조에서는 PlayerStart의 스폰 Yaw를 그대로 물고 있을 뿐이다.
`GetCameraYaw()`는 `USpringArmComponent::GetTargetRotation()`을 쓰므로 `bInherit*` / `bUsePawnControlRotation`
설정 변화를 자동으로 반영하고, 랙이 걸리지 않은 목표 회전값이라 카메라가 도는 도중에도 조작이 밀리지 않는다.

카메라 회전 기믹 API (BP 노출, 시크릿 요소용):
`SetCameraYaw(Yaw, bInstant)` / `AddCameraYaw(Delta, bInstant)` / `GetCameraYaw()`.
`Tick`의 `UpdateCameraYaw()`가 `CameraYawInterpSpeed`로 최단 방향 보간한다. 이동은 자동으로 따라간다.

PIE 콘솔 튜닝 명령 (`ADSCharacterPlayer`의 Exec):

| 명령 | 용도 |
|---|---|
| `DSCam <FOV> <ArmLength>` | 0 이하 인자는 현재 값 유지. 적용 후 FOV/Arm/ViewWidth를 로그로 출력 |
| `DSCamYaw <각도>` | 절대 각도로 회전 |
| `DSCamRotate <델타>` | 상대 회전, 인자 생략 시 90도 |

### 4. 인터페이스가 확장 지점이다

- `IDSActivatableInterface::Activate()` — 버튼(`ADSButton`의 `LinkedActors`), 트리거, 레벨 이벤트가
  **전부 이 하나로** 대상 액터를 깨운다. 새 기믹은 이걸 구현하면 세 발동 경로에 그대로 꽂힌다
- `IDSInteractableInterface` — `Interact(Player)` / `OnFocus` / `OnFocusLost`. `UDSInteractionComponent`가 포커스를 관리
- `IDSAttackTraceInterface` — 무기가 캐릭터에게 트레이스를 위임(`StartAttackTrace`/`PerformAttackTrace`/`EndAttackTrace`)
- `IDSDamageableInterface` — `ApplyDamage` / `ApplyDamageWithKnockback`

### 5. 서브시스템

- `UDSUIManagerSubsystem`, `UDSTutorialSubsystem` — **LocalPlayerSubsystem** (`ULocalPlayer::GetSubsystem<T>()`)
- `UDSIndoorStateSubsystem` — **GameInstanceSubsystem**. `ADSIndoorAreaTrigger` / `ADSIndoorVisibilityManager`와 함께
  실내 진입 시 천장·벽 가시성을 처리한다. 원거리 카메라라 오클루전 문제가 생기면 여기부터 볼 것

### 6. 입력 모드 전환

`ADSPlayerController::ChangeInputMode(EPlayerInputMode::{Gameplay, Cinematic, UI})` 가 IMC를 갈아끼운다.
단, `ADSCharacterPlayer::BeginPlay`도 IMC를 추가하고 그 직후 `SetCharacterControl()`이 `ClearAllMappings()`를 부른다 —
**실제로 활성인 IMC는 `DSC_Move`가 들고 있는 것**이지 `DefaultMappingContext`가 아니다.

### 7. 게임 모드 배선

`ADSGameModeBase`가 `/Game/Character/BP_CharacterTest` + `/Game/Character/BP_PlayerController`를 쓴다.
`DefaultEngine.ini`의 `GlobalDefaultGameMode`는 빈 스텁(`ADreamscapeGameModeBase`)을 가리키므로 맵의 GameMode 오버라이드가 실제로 쓰이는 값이다.

## 알려진 거친 부분

- 에셋 참조가 생성자의 **하드코딩 문자열 경로**(`ConstructorHelpers::FObjectFinder`)다.
  Content에서 에셋을 옮기거나 이름을 바꾸면 C++이 조용히 널을 물고 시작한다
- `ADSCharacterPlayer::BeginPlay`는 `SetShowMouseCursor(true)`, `ApplyGameplayMode`는 false — 커서 상태가 서로 다툰다.
  `RotateCharacterToMouseCursor()`가 커서 위치 디프로젝션에 의존하므로 관련 버그를 볼 때 여기를 의심할 것
- `ADSEnemyCharacterBase`는 `ADSCharacterBase`가 아니라 `ACharacter`를 직접 상속한다. 플레이어와 공용 코드가 없다

## 현재 작업 (브랜치 `PlayerCamera`, 2026-09-07 기준 · 미커밋)

기획서의 시점/이동 규칙을 코드에 정합화하는 중. 적용 완료(빌드 통과, **PIE 검증 미완**):

- `UDSCharacterControlData::FOV` 기본값 90 → 8, UI 슬라이더 4~10 제한
- `Move()`의 기준축을 `GetControlRotation()` → `GetCameraYaw()` 로 교체
- 카메라 Yaw 회전 API + `UpdateCameraYaw()` 보간, 회전 랙 off
- `ADSCharacterPlayer::Tick()`이 `Super::Tick()`을 호출하지 않아 **BP Event Tick이 죽어 있던 문제** 복구.
  BP_CharacterTest의 Event Tick에 로직이 있었다면 이제 실행되므로 부작용 확인이 필요하다
- 부수 수정: SwordAttack 에셋 로드 조건문 오타, `GetCamera()` 정의 누락, `Look()`/`LookAction` 데드코드 제거

남은 일:

1. `DSCamRotate` 추가분이 **아직 컴파일되지 않았다** — 빌드 필요
2. PIE 검증: FOV 8 프레이밍(캡슐이 소형 스케일이라 암 길이 재조정이 필요할 수 있음),
   PlayerStart를 Yaw 90도 돌려놓고 W가 여전히 화면 위쪽으로 가는지, `DSCamRotate` 회전 중/후 조작 일치
3. 에디터 작업: `DSC_Move`에 FOV=8 명시 저장, 레벨 인스턴스에 Yaw 45 적용,
   디렉셔널 라이트 Dynamic Shadow Distance 점검(카메라가 12500 떨어져 있다)
