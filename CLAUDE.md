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
- `IDSAttackTraceInterface` — `UDSAnimNotifyState_Attack`이 **소유 액터**를 이 인터페이스로 캐스팅해
  `StartAttackTrace`/`PerformAttackTrace`/`EndAttackTrace`를 준다. 판정은 양쪽 다 `ADSSwordWeaponBase`의 **소켓 캡슐 트레이스**.
  플레이어는 `UDSPlayerCombatComponent`가 `CollectHits()`로 닿은 액터만 받아 `Stats × 무기 배수`로 피해를 주고,
  보스(`ADSBoss01`)는 `PerformTrace()`가 `DamageAmount`를 직접 준다
- `IDSDamageableInterface` — `ApplyDamage` / `ApplyDamageWithKnockback`. 구현체는 전부 `UDSVitalityComponent`에 위임한다

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

### 8. 캐릭터 컴포넌트 구조 (Notion `문서/플레이어 캐릭터`·`적 캐릭터` 기준으로 개편 중)

컴포넌트는 전부 `Components/` 바로 아래에 평면으로 둔다. 소유자별 하위 폴더를 만들지 않는다.

```
UDSCharacterStatsComponent   공통. BaseMaxHealth(적 BP에서 편집) + OnStatsChanged
└─ UDSPlayerStatsComponent   UDSPlayerDataAsset에서 초기화. 업그레이드 포인트(FDSPlayerStatsSnapshot) · 파생 스탯 계산
UDSVitalityComponent         공통. 현재 체력 · 무적 · 사망 · 이벤트. virtual SanitizeDamage()가 정수(플레이어)/실수(적) 규칙을 가른다
└─ UDSPlayerVitalityComponent  + 스테미나(틱 자동 회복), 정수 반올림
UDSPlayerCombatComponent     콤보(커맨드 패턴) · 피해 계산 · 무기 장착. 판정은 무기(ADSSwordWeaponBase)의 소켓 트레이스에 위임. 파생값 = Stats × UDSWeaponItemData
```

- **소유 규칙**: 최대치는 Stats, 현재값은 Vitality. 파생값(피해·배속·사거리·회복시간)은 저장하지 않고 매번 계산
- **초기화 순서**: `PostInitializeComponents()`에서 `Stats->InitializeStats()` → `Vitality->Initialize(Stats)`. 플레이어·적 동일
- **컴포넌트는 FSM을 직접 부르지 않는다.** 캐릭터가 `OnStaggered`/`OnDeath`/`OnAttackSequenceEnded`를 구독해 FSM에 넘긴다
- **무적은 상태 클래스가 켠다** (`UDSPlayerState_Roll/_Hit::Enter/Exit` → `SetInvincible`). 적 상태는 안 부르므로 "경직 중에도 피격"이 자동으로 성립
- **이동 속도 함정**: `SetCharacterControlData()`가 `MaxWalkSpeed`를 덮어쓰므로 스탯 배수는 그 뒤에 `ApplyMoveSpeedFromStats()`로 곱한다
- **UE 몽타주 함정**: 블렌드 아웃은 배속과 무관한 실제 초 단위라, 배속이 오르면 콤보 창이 블렌드 아웃에 갇힌다.
  `UDSPlayerCombatComponent`가 창 열림을 블렌드 아웃 직전으로 클램프하고, 그래도 멈춰 있으면 재시작한다.
  `Montage_GetIsStopped`는 블렌드 아웃 **시작** 순간부터 true다 (종료 델리게이트는 끝나야 온다)
- 적 최대 체력은 아직 데이터 에셋이 없어 각 BP의 `StatsComponent → Base Max Health`에서 편집한다 (Ghoul 3 · Boss01 30). Phase 7에서 이관
- **사거리 스탯은 미구현.** `UDSWeaponItemData::AttackRange`·`GetAttackRangeMultiplier()`는 남아 있지만 아무도 읽지 않는다.
  Phase 2에서 플레이어 앞 히트박스로 만들었다가 사용자 결정으로 소켓 트레이스로 되돌렸다(2026-09-20)

PIE 콘솔 (`ADSCharacterPlayer` Exec): `DSVital` · `DSDamage <양>` · `DSStamina <양>` · `DSUpgrade <Attack|SubAttack|Speed|Mobility> <n>` · `DSCombat` · `DSWeaponTrace <0|1>`

## 알려진 거친 부분

- 에셋 참조가 생성자의 **하드코딩 문자열 경로**(`ConstructorHelpers::FObjectFinder`)다.
  Content에서 에셋을 옮기거나 이름을 바꾸면 C++이 조용히 널을 물고 시작한다
- `ADSCharacterPlayer::BeginPlay`는 `SetShowMouseCursor(true)`, `ApplyGameplayMode`는 false — 커서 상태가 서로 다툰다.
  `RotateCharacterToMouseCursor()`가 커서 위치 디프로젝션에 의존하므로 관련 버그를 볼 때 여기를 의심할 것
- `ADSEnemyCharacterBase`는 `ADSCharacterBase`가 아니라 `ACharacter`를 직접 상속한다. 공유되는 것은 컴포넌트뿐이다
- `ADSCharacterPlayer::OnDeath()`와 `ADSBoss01::OnDeath()`는 `Super::OnDeath()`를 부르지 않는다 (의도적으로 둔 현행 동작)
- `UDSPlayerState_Fall::OnDeath()`가 빈 구현이라 낙하 중 사망이 처리되지 않는다. `EPS_Land`는 진입 경로가 없는 사문화 상태
- `UDSCharacterComboActionData::MontageSectionNameProfix` 오타 — 고치면 `DA_ComboAttack` 값이 날아가므로 그대로 둔다

## 현재 작업 — 캐릭터 구조 개편 (브랜치 `PlayerCamera`, 2026-09-15 기준)

Notion `Dreamscape/문서/플레이어 캐릭터`·`적 캐릭터`의 컴포넌트 구조로 갈아끼우는 중.
**페이즈 게이트 방식**: 한 페이즈 구현 → 빌드 → PIE 검증 항목 제시 → **사용자가 PIE로 검증** → 통과해야 다음 페이즈.
상세 계획·결정 기록·게이트 항목은 `C:\Users\User\.claude\plans\melodic-skipping-brooks.md`에 있다 (새 세션이면 먼저 읽을 것).
코드 구조 문서(아티팩트, 같은 URL에 제자리 갱신): `https://claude.ai/code/artifact/9eb4fb38-4812-41df-97c5-0ca6cdd6eab6`

| Phase | 내용 | 상태 |
|---|---|---|
| 1 | 스탯·체력을 컴포넌트로 분리, `UDSPlayerDataAsset` | ✅ 커밋 `6ec53cf` |
| 1.5 | Vitality/Stats 공통 베이스, 적 캐릭터 교체, `Components/` 평면화 | ✅ 커밋 `6ec53cf` |
| 2 | `UDSPlayerCombatComponent`, 무기 데이터 확장, 회복 시간 | ✅ 게이트 통과 · **미커밋** |
| 2.1 | 판정을 히트박스 → 무기 소켓 트레이스로 되돌림. 사거리 미구현으로 명시 | 빌드 통과 · **게이트 미검증** |
| 3a | 보조 공격 공통 뼈대 + 01(화살), `EDSDamageType` 도입 | **계획 완료 (2026-10-05)** · 미착수 |
| 3b | 보조 공격 02 부채꼴 · 03 포물선 폭발 · 04 대쉬 | 개요만 |

### Phase 2 마무리 상태 (다음 세션 시작점)

- 게이트 7개 중 6개 통과. **1번의 `DA_Sword02` 콤보 4회는 4섹션 몽타주가 없어 보류** — 코드는 `MaxComboCount`가 섹션 수를 넘어도 상한에서 멈추므로 깨지지 않는다
- 배속 1.5×에서 콤보가 끊기던 문제는 해결됨. 근본 원인은 `AM_SwordAttack`의 Blend Out이 배속을 안 따라가는 것이었고,
  사용자가 에셋의 **Blend Out Trigger Time = 0.0**으로 해결. 코드 쪽 클램프·재시작 안전장치는 남겨 둠
- `UDSWeaponItemData::DamageAmount`는 보스 경로 전용으로 남겼다 (`DamageMultiplier`가 플레이어용). Phase 7에서 분리
- **Phase 2.1 (2026-09-20)**: 히트박스를 걷어내고 `ADSSwordWeaponBase::CollectHits()`(판정만) + `PerformTrace()`(보스 레거시, 피해까지)로 분리.
  플레이어 피해는 `UDSPlayerCombatComponent::PerformAttackTrace()`가 `CollectHits` 결과에 `Stats × 배수`로 준다. **PIE 게이트 6개 미검증** (플랜 파일 참조)

### 다음 세션에서 할 일

1. **Phase 2.1 게이트 검증** → 통과하면 **Phase 2 + 2.1 함께 커밋** (사용자가 GitHub Desktop으로)
2. **Phase 3a 착수** — 계획은 플랜 파일 **"Phase 3 — 보조 공격"** 절에 0~8단계로 확정되어 있다. 새 세션은 그 절부터 읽을 것.
   - 사용자 확정: 3a·3b 분할 · 숫자 키 1~4 슬롯 전환 · 충전 완료 후 떼면 발동(미완료 시 취소, 스테미나 소모 없음, 충전 중 이동 불가) · 강화는 범위 밖
   - **협업 방식**: 사용자가 "써줘"라고 하기 전에는 소스 파일에 코드를 넣지 않는다. 단계 순서대로 "어느 파일 어디에 · 무슨 코드 · 왜"를 챗에서 보여주고 사용자가 직접 입력한다.
     단계마다 빌드가 통과하는 상태로 끊는다. 사용자가 입력했다고 하면 `git diff` 리뷰 → 빌드 → 지적
   - 1단계(`EDSDamageType` 도입)가 가장 넓게 퍼진다 — 인터페이스 구현체 3개 + 호출부 약 10곳. 호출부별로 넘길 종류는 플랜 파일 표 참조
3. 그다음 Phase 3b → 4(인벤토리 3분할) → 5(FSM 명칭·`CharacterStateComponent` 판단) → 6(입력 IA 나머지) → 7(적 스탯·데이터 에셋)
4. Phase 7에서 함께 정리할 것: `UDSCharacterDataAsset` 공통 베이스(최대 체력·기본 공격력·기본 이동 속도 3필드) 도입 여부는 논의 끝났고 "얇게 만든다"로 잠정 합의.
   플레이어 기준 이동속도가 `DSC_Move`(카메라 프리셋)에 있는 이중 출처 문제도 이때 정리

### 기획서에서 아직 비어 있는 결정 (코드는 잠정값으로 돌고 있음)

스테미나 자동 회복 규칙(초당 1.0) · 무기 사거리 수치 · 기본 공격 충전(휠 홀드)의 효과 · 보스 경직 임계값 ·
데미지 감소율/넉백 저항치의 적용 주체 · 적 `CharacterStateComponent` 공통화 여부 · 보스 사망 연출(레그돌 없음 유지 중) ·
보조 공격 강화 효과의 해금 경로 · 3b 수치(부채꼴 반지름·각도, 폭발 반지름·자해량, 대쉬 거리·무적 여부) · 보조 공격 애니메이션 에셋
