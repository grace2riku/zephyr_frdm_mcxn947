# 使用バージョンと開発環境の記録

調査・選定日：2026-10-10（日本時間）。対象：[段階0・イシュー #1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1)。

状態：段階0の選定・記録は完了。段階0時点では導入・ビルド・実機確認は未実施だった。段階1でCodexが環境構築とHello Worldのビルド・書き込み・UART・GDB確認を事前検証し、その後ユーザー本人からも各操作の成功結果を受け取った。[事前検証と本人の実習結果](../01_hello_world/README.md)を記録済み。2026-10-10にユーザー本人から「段階1の確認はOK」との確認を受け、その後、本人のPR確認と承認を経てPR #17をマージ、イシュー #2をcompletedとしてクローズ済み。本書の選定根拠、Codexの事前検証、ユーザー本人の実習結果を区別する。段階2もこの固定環境を継続する。

## 選定の結論

WindowsネイティブのPowerShellとVS Codeを使い、Zephyr **v4.4.2**、Zephyr SDK **1.0.1のGNU Armツールチェーン**を採用する。FRDM-MCXN947のCPU0と内蔵Flashから始める。書き込み・デバッグはオンボードMCU-LinkのCMSIS-DAPと、既存のLinkServer **1.4.85**を使う。

学習中に同じ条件で再現できることを優先する。Zephyrだけを固定しても、SDKやPythonパッケージが変われば結果やエラーが変わり得る。そのため、Zephyrのコミット、モジュールのコミット、SDK、ホストツール、Pythonパッケージを別々に記録する。

## 現在のPC環境：実測結果

PATH、実行ファイルのバージョン、Windowsのインストール情報、標準的な配置先を調査した。「PATHにない」と「未インストール」は区別する。全ドライブの網羅的な検索は行っていない。

| 項目 | 調査結果 | 判断への影響 |
|---|---|---|
| OS | Windows 11 Pro 25H2、64ビット、ビルド26200.9457 | Windowsネイティブを採用。レジストリのProductNameはWindows 10 Proだったが、CIMのCaptionはWindows 11 Proだったため後者を使用 |
| CPU／RAM | AMD Ryzen 9 7940HS、論理プロセッサ16、OSから見えるRAM約61.8 GiB | ホスト用ツールはWindows x86-64版。PCのRAMとボードのRAMは別 |
| Cドライブ空き | 約712.3 GiB（調査時点） | SDKとワークスペースを追加する余裕がある |
| PowerShell | Codexの作業用ランタイムは7.6.5。2026-10-11に本人の元の端末はWindows PowerShell 5.1（Build 26100、Revision 9444）と確認。その後、本人も7.6.5を起動し版を確認済み | 段階0の7.6.5という実測と本人の端末を区別する。7.6.5の実体は`C:\Users\grace\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\powershell\pwsh.exe`。通常のProgram Filesの配置には見つからなかった。段階2で本人がこの実体を起動し、Major 7、Minor 6、Patch 5を確認した |
| Git | 2.52.0.windows.1、実行確認済み | 既存版を使用 |
| VS Code | 1.139.1、x64。CLIとアプリのpackage.jsonで確認 | 既存版を使用 |
| VS Code拡張 | C/C++ 1.34.4。調査した拡張フォルダにCortex-Debugは見つからない | まずCLIのGDBを使い、GUI拡張は必要時に版・設定を記録 |
| Python | `python --version`は3.10.3。別配置の3.13.3も実行確認 | 既定の3.10.3は採用Zephyrの最低要件3.12を満たさない |
| Python Launcher | 通常権限の`py --list-paths`で3.13／3.10を確認 | 導入後は`py -3.12`で使用版を明示 |
| west／CMake／Ninja | PATHと調査したインストール情報に見つからない | 段階1で指定版を導入 |
| Zephyr SDK | PATH・調査した標準配置先・Zephyr関連環境変数から既存設定を確認できない | 段階1で指定版を導入し使用先を明示 |
| LinkServer | `C:\nxp\LinkServer_1.4.85\LinkServer.exe`、`-v`でv1.4.85／Build 85を確認。PATHにはない | 既存版を使用しセッションのPATHに追加 |
| MCU-Link更新パッケージ | `C:\nxp\MCU-LINK_installer_3.133`とインストール情報あり | PCにある更新ツールの版であり、ボード上の版は未確認 |
| MCUXpresso IDE | 11.9.0、Build 2144 | 既存環境として記録。Zephyrのビルドはwest＋CMakeで行う |
| 既存Armコンパイラ | PATHはGNU Arm Embedded 10.3-2021.10の配置を指す。Arm GNU Toolchain 13.2.Rel1のインストール情報もあり | 複数版の混在を避け、Zephyr SDKを明示 |
| J-Link | 7.94bのインストール情報あり、PATHにはない | 初期構成はボード既定のCMSIS-DAP／LinkServerに統一 |
| シリアル端末 | Tera Term 5.4.0。実行ファイルのProductVersionでも確認 | 既存版を使用 |
| 補助ツール | 段階0時点では7-Zip 25.01あり。gperfとdtcはPATHにない | 7-Zipを再利用。段階1でgperf 3.1とDTC 1.6.1を別導入（DTCは実物確認により訂正） |
| WSL | Ubuntuディストリビューション登録あり | 代案はあるが初期開発はWindowsネイティブに統一 |
| Rust／Cargo | PATHに見つからない | 段階14で選定 |
| シリアルポート | Win32_SerialPortでは列挙結果なし | 接続やドライバの正常性は判断できない。段階1でCOM番号とプローブを確認 |

制限付き実行ではCIMへのアクセスやLinkServer起動に失敗したが、通常権限の読み取り確認ではCIMとLinkServer `-v`が成功した。制限付き実行のエラーだけでPCやLinkServerが壊れているとは判断しない。旧Armコンパイラは制限付き起動に失敗しており、その版の実行可否は未確認。

主な読み取り確認コマンド（PowerShell）：

```powershell
Get-CimInstance Win32_OperatingSystem | Select-Object Caption, Version, BuildNumber, OSArchitecture
Get-CimInstance Win32_ComputerSystem | Select-Object TotalPhysicalMemory
Get-ItemProperty 'HKLM:\SOFTWARE\Microsoft\Windows NT\CurrentVersion' |
    Select-Object DisplayVersion, CurrentBuild, UBR
Get-Command git, python, py, code -ErrorAction SilentlyContinue
Get-Command west, cmake, ninja, LinkServer, gperf, dtc -ErrorAction SilentlyContinue
git --version
python --version
py --list-paths
& 'C:\Users\grace\AppData\Local\Programs\Python\Python313\python.exe' --version
& 'C:\nxp\LinkServer_1.4.85\LinkServer.exe' -v
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
wsl --list --quiet
```

## ツールと依存関係

| 項目 | タグ／バージョン／コミット | 選定理由・確認結果 |
|---|---|---|
| 開発ホストOS | Windows 11 Pro 25H2 x64、PowerShell 7.6.5 | 既存USBドライバ・LinkServer・端末を使える。WSLへUSB接続を渡す作業を初期学習に追加せずに済む |
| エディタ／IDE | VS Code 1.139.1、C/C++ 1.34.4 | 導入済み。エディタとビルド機構を分けると、何がコンパイルを実行するか学びやすい |
| Zephyr | **v4.4.2**／`dccb09599635bdff17633fa7e9dab014b91dce90` | 公開済み4.4系修正版。採用タグでボード・LCD・カメラ・サンプルを確認。mainやRCではなくコミットを固定 [Z1–Z5] |
| Zephyr SDK | **1.0.1**、Windows x86-64、GNU `arm-zephyr-eabi`＋ホストツール | 採用タグの`SDK_VERSION`指定版。ボードCPUがArmでもホスト用配布物はWindows x86-64を選ぶ [Z6, T1] |
| C／C++／GDB | SDK付属GCC 14.3.0、Binutils 2.43.1、GDB 16.2 | SDK単位で管理し、既存`arm-none-eabi`版と混在させない。Slint用C++設定は段階14で再確認 [T1] |
| Python | **CPython 3.12.10、Windows x64**を新規導入 | 最低要件3.12を満たす。公式Windowsインストーラがある3.12系の最終通常修正版で、導入手順を揃えやすい。既存3.10／3.13と併存 [Z7, T2] |
| west | **1.5.0**、専用venv内 | 正式公開版。採用Zephyrの`west>=0.14.0`を満たしPython 3.12対応。依存導入時の版変更をconstraintsで防ぐ [Z8, T3] |
| CMake | **3.31.8、Windows x64** | 採用タグのGetting Startedの最低3.20.5を満たす3系修正版。4系への移行要因を初期環境に追加しない [Z7, T4] |
| Ninja | **1.13.1、Windows x64** | 公開済み正式版。CMakeのNinjaジェネレータでビルド処理を統一 [T5] |
| Git | **2.52.0.windows.1** | 実行確認済みの既存版。現時点で変更が必要な根拠がない |
| LinkServer | **1.4.85**を初期採用 | 同梱資料にFRDM-MCXN947対応あり。採用タグの`board.cmake`に1.4.85以前を考慮したメモリ領域指定あり。段階1でflash／debugの実動作を確認 [Z9, L1] |
| MCU-Linkファームウェア | **工場出荷CMSIS-DAPを維持**。実装版は段階1で記録 | PCの更新パッケージ3.133とボード上の版は別物。必要時のみ更新し、更新前後を記録 [Z2, L2] |
| シリアル端末 | **Tera Term 5.4.0** | 導入済みで115200 bps／8N1を設定できる |
| gperf | **3.1、Windows x64** | ZephyrのWindows導入手順で案内される`oss-winget.gperf`の提供版を固定 [Z7, T6] |
| Devicetree compiler | **oss-winget.dtc 1.6.1、Windows x64** | 段階1の実物確認でWindows SDK内にDTCがないと判明したため、当初の「SDK付属1.6.0」を訂正。採用Zephyrの公式Windows手順が案内する配布物を追加。最低1.4.6を満たす [Z7, T8] |
| 展開ツール | **7-Zip 25.01** | 既存版を再利用 |
| NT-Shell | 段階7で選定 | 取得元・タグ・コミット・ライセンス・取り込み方法を記録 |
| Slint／Rust／Cargo | 段階14で選定 | Slintの採用タグとZephyrの組合せ、Rust要件を確認して固定 |

Python 3.12.10を最新のセキュリティ修正版とは扱わない。調査時点で3.12.15が公開されているが、公式配布はソースのみでWindowsインストーラがない [T7]。今回はWindowsの初期構築の再現性を優先して3.12.10を選ぶ。段階1でパッケージ解決を確認し、保守された新しいPythonへの移行が必要になった場合は下記の変更手順で改定する。既存3.13.3も、最低要件を満たすだけでは採用Zephyrの全依存パッケージの動作確認済みとは言えない。

### Zephyrの対応を確認した根拠

LTSという名前だけで選ぶのではなく、今回の機材を同じタグで扱えることを優先した。旧LTS版については同等の対応がないと断定せず、今回の採用比較では未検証とする。

| 対象 | v4.4.2で確認した内容 | 残る検証 |
|---|---|---|
| FRDM-MCXN947 | CPU0単独実行、初期ターゲット、LinkServer既定、J17 UARTを記載 [Z2] | 段階1でHello World・書き込み・ブレークポイント |
| LCD_PAR_S035 | ボード資料にFlexIO／MIPI DBI対応。シールド資料にCPU0＋`lcd_par_s035_8080`の表示サンプル [Z2, Z3] | 段階10で現物接続・表示・色 |
| OV7670 | ボード資料にSmartDMA映像入力。シールド`dvp_20pin_ov7670`あり [Z2, Z4] | 段階12でピン配列・画像取得 |
| カメラ＋LCD | `samples/drivers/video/capture`にCPU0＋両シールドの組合せ例あり [Z5] | 段階13で表示・RAM使用量・更新 |

README初期資料のカメラサンプルURLは`samples/subsys/video/capture`だったが、採用版では **`samples/drivers/video/capture`**。シールド資料の短いボード名をそのまま使わず、本プロジェクトではCPU0まで指定する。

搭載RAMは512 KBだが、採用タグの資料でCPU0用`srama`は320 KBとされる [Z2]。512 KB全部を自由に使えると仮定せず、画像・GUI段階で生成Devicetree、mapファイル、バッファサイズを確認する。

LinkServer 1.4.85はIntel HEX対応が追加された1.5.30より前だが、採用Zephyrのrunnerは版を判定しBIN／ELFへ切り替える [Z10]。まず既存版を使い、HEXが必要な構成や実動作上の問題が出たら更新する。

## 機材と接続

- FRDM-MCXN947の基板リビジョン：未確認。
- 初期ビルドターゲット：`frdm_mcxn947/mcxn947/cpu0`。
- 初期構成：内蔵Flashを使用。CPU1、QSPI起動、TF-Mは初期構成へ追加しない。
- PCとボードの接続：データ通信対応USB Type-Cケーブルで **J17** へ接続。オンボードMCU-Link経由のSWD／UARTを利用 [Z2]。
- 書き込み・デバッグ：CMSIS-DAP＋LinkServer。CLIの`west flash -r linkserver`、`west debug -r linkserver`から始める。SDK付属GDBで`main`に停止しステップ実行を確認する。
- コンソール：Tera Term、**115200 bps、8 data bits、no parity、1 stop bit、フロー制御なし**。COM番号・USBドライバ・プローブ実装版は段階1で確認する。
- LCD：ボードのLCD並列コネクタと`lcd_par_s035_8080`を採用。現物のインターフェース選択・向きを段階10で確認する。
- カメラ：DVP 20-pin互換OV7670と`dvp_20pin_ov7670`を採用。資料は18本の有効ピンを示すため、名称だけで接続せず段階12で現物と配列を照合する [Z4]。
- LCD／カメラの現物状態：未確認。段階10／12で詳細を記録する。

## 依存関係とワークスペース

依存ソースは学習用アプリのGitリポジトリの外へ置く。パスを短めにし空白・日本語を避ける。以下は段階0で決めた保存先で、段階1で構築済み。

| 用途 | 保存先 |
|---|---|
| 学習用アプリ・記録 | `C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947`（現在のリポジトリ） |
| Zephyr＋モジュール | `C:\Users\grace\zephyrproject-mcxn947` |
| Python仮想環境 | `C:\Users\grace\zephyrproject-mcxn947\.venv` |
| SDK | `C:\Users\grace\zephyr-sdk-1.0.1` |
| 段階1ビルド出力 | `C:\Users\grace\zephyrproject-mcxn947\build\01_hello_world` |

段階1でPython 3.12.10、CMake 3.31.8、Ninja 1.13.1、gperf 3.1を指定して導入する。wingetの無指定導入で最新版を選ばず、指定版が提供されない場合は上記の公式配布物を使う。インストーラ／アーカイブのファイル名とSHA-256も記録する。環境変数は開発セッションで明示し、既存の別用途のコンパイラ設定を上書きしない。

### 段階1の初回取得手順（段階0時点で未実行）

前提：ホストツール導入後、新しいPowerShellで`py -3.12 --version`が3.12.10を返すこと。各コマンドの成功を確認して次へ進む。

```powershell
$zephyrWorkspace = 'C:\Users\grace\zephyrproject-mcxn947'
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
New-Item -ItemType Directory -Path $zephyrWorkspace -Force
py -3.12 -m venv "$zephyrWorkspace\.venv"
& "$zephyrWorkspace\.venv\Scripts\Activate.ps1"
python --version
python -m pip install 'west==1.5.0'

west init -m https://github.com/zephyrproject-rtos/zephyr `
    --mr v4.4.2 -o=--depth=1 $zephyrWorkspace
if ($LASTEXITCODE -ne 0) { throw 'west init failed' }
if ((git -C "$zephyrWorkspace\zephyr" rev-parse HEAD) -ne `
    'dccb09599635bdff17633fa7e9dab014b91dce90') { throw 'Zephyr SHA mismatch' }
Set-Location $zephyrWorkspace
west update --narrow -o=--depth=1
west zephyr-export

# Zephyrの依存導入によってwestの版が変わらないよう制約を与える。
'west==1.5.0' | Set-Content -Encoding ascii '.\python-constraints.txt'
python -m pip install -c '.\python-constraints.txt' @((west packages pip) -split ' ')
python -m pip check
west --version

Set-Location "$zephyrWorkspace\zephyr"
west sdk install --version 1.0.1 --gnu-toolchains arm-zephyr-eabi `
    --install-dir 'C:\Users\grace\zephyr-sdk-1.0.1'

$env:ZEPHYR_TOOLCHAIN_VARIANT = 'zephyr'
$env:ZEPHYR_SDK_INSTALL_DIR = 'C:\Users\grace\zephyr-sdk-1.0.1'
$env:ZEPHYR_BASE = "$zephyrWorkspace\zephyr"
$env:Path = 'C:\nxp\LinkServer_1.4.85;' + $env:Path
git -C "$zephyrWorkspace\zephyr" rev-parse HEAD
cmake --version
ninja --version
gperf --version
LinkServer -v
```

venvの有効化が実行ポリシーで拒否された場合は、段階1で既存設定を確認して通常のPowerShell／cmdでの手順を決める。ここではPCの実行ポリシーを変更していない。段階1では[セッション設定スクリプト](../01_hello_world/scripts/env.ps1)でvenvの実行ファイルとツールのPATHを明示する。Windows SDKの実物にDTCがないため別途1.6.1を導入する。

2026-10-10（段階1での訂正）：west 1.5.0の`init --mr`はブランチ名またはタグ名を受け取る。コミットSHAを渡す当初手順は失敗したため、上記をタグ取得＋SHA検証へ修正した。`--depth=1`は履歴量を抑えるだけで、使用ソースの版は変えない。実行済みの環境構築・検証結果は[段階1](../01_hello_world/README.md)を参照。

### 段階1のビルド・実行・デバッグ手順（段階0時点で未実行）

上記と同じセッションでZephyr付属Hello Worldを使用する。

```powershell
Set-Location "$zephyrWorkspace\zephyr"
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    -d "$zephyrWorkspace\build\01_hello_world" samples/hello_world
west flash -d "$zephyrWorkspace\build\01_hello_world" -r linkserver
west debug -d "$zephyrWorkspace\build\01_hello_world" -r linkserver
```

期待する結果：ビルド完了、UARTに`Hello World! frdm_mcxn947/mcxn947/cpu0`が出ること、GDBでブレークポイント・ステップ実行ができること。段階0時点では未実施だった。段階1でCodexがpyserialによるUART受信とGDB確認を事前検証し、コマンド、実行ディレクトリ、ログと未解決事項を記録した。その後、ユーザー本人のTera TermによるUART表示、GDBのmain停止・next／step・変数・メモリ確認と終了操作も確認済み。2026-10-10にユーザー本人から「段階1の確認はOK」との確認を受けた。

### 依存モジュールの固定と再構築

ZephyrはOS・ドライバ・ビルド設定、SDKはコンパイラ等の配布物、westは複数のGitリポジトリを管理する道具、`west.yml`は取得先とリビジョンの一覧である。例えばNXP HALは、PCにあるMCUXpresso SDKを自動で使うのではなく、採用Zephyrのmanifestに従って取得する。

1. Zephyrを上記コミットに固定し、段階1で`git rev-parse HEAD`との一致を確認する。
2. そのコミットの`west.yml`とimportされるsubmanifestで`west update`する。モジュールだけを手動で最新版へ切り替えない。例：`hal_nxp`は`7554bc0f6f6c284972eba7b2c17b16fbb9cc0ad0` [Z11]。
3. 取得成功後に`west manifest --freeze --active-only`でimportを展開した有効モジュールのSHAを保存する。公式manifestの既定グループに従い、無効なoptional／testing／babblesimは取得していないため除く。`self`にはZephyr本体のSHAが固定されないため、本書のZephyr SHAを必ず併用する [Z12]。
4. 採用ZephyrのrequirementsでPython依存を解決し、専用venvの全パッケージを`pip freeze`で保存する。初期westの版はconstraintsで固定する。
5. 再構築は同じPython／SDK／CMake／Ninjaを用意し、同じZephyr SHAで取得する。保存したfrozen manifestとの差、各リポジトリのローカル変更、`pip check`を確認する。相違があれば再現成功とは扱わない。

段階1で取得・解決後に保存する証跡（保存済み）：

```powershell
Set-Location $zephyrWorkspace
west manifest --freeze --active-only -o "$learningRepo\stages\00_versions\west-frozen.yml"
west list -f '{name} {revision} {sha} {path}' |
    Set-Content -Encoding utf8 "$learningRepo\stages\00_versions\west-projects.txt"
python -m pip freeze --all |
    Set-Content -Encoding utf8 "$learningRepo\stages\00_versions\python-requirements-lock.txt"
```

これらは段階0では空の固定ファイルを作らず、段階1で実際の取得・解決結果から生成した。再取得時は新しい専用venvへ`west==1.5.0`を入れ、保存した`python-requirements-lock.txt`を`pip install -r`で復元する。`pip freeze --all`はpip等も含めるが、SDKや外部ツール、配布アーカイブのハッシュまで固定するものではないため、版とSHA-256は別に記録する。

リポジトリに残すのはアプリ、設定、手順、固定情報、検証記録。Zephyr本体、モジュール、SDK、venv、ビルド出力はコミットしない。

## Slint導入時の再確認方針

段階14で、採用するSlintタグ／コミットのC++ MCU手順、Zephyrデモのmanifest、Rustツールチェーン、C++標準・例外等の設定を照合する。`master`や`latest`をそのまま固定条件としない。

今回読んだ現行デモREADMEではRust nightlyとSDK 1.0.1が案内されているが、動作対象の例にFRDM-MCXN947は明記されていない [S1]。今回のボードで動作保証されたという意味ではない。段階14で次を確認する。

- SlintとZephyr v4.4.2のAPI・manifestの整合、Rustターゲット、必要なら日付付きnightly、Cargo.lockとSlintの正確な版。
- WindowsでのPCデモ、クロスビルド、ソフトウェアレンダラ、LCD_PAR_S035への描画接続。
- CPU0のRAM範囲、描画バッファと画像資産、Flash／RAM使用量。全画面二重バッファを最初から仮定しない。
- 既存環境と別のワークスペースで試し、Zephyr更新が必要なら完了済み段階を再ビルド・実機確認して採用する。

## バージョン変更の方針

変更前後のタグ・コミット・版、変更理由、影響する段階、再ビルド結果、書き込み／デバッグ／実機結果を変更履歴へ追記する。初回ビルドは`-p always`を使い、古いキャッシュの影響を避ける。SDKとZephyrの更新は別項目として扱う。ビルドのみ成功した場合は実機確認済みとは記録しない。

段階0時点の引継ぎ未確認事項は、ホストツール導入・Python依存解決、SDK使用パス、基板リビジョン、COM番号とUSBドライバ、MCU-Link実装版、Hello World、LinkServerでの書き込みとGDB接続だった。段階1で基板の印刷リビジョンを除き確認済み（[実測結果](../01_hello_world/README.md)）。LCD／カメラの現物接続は段階10／12、Slint互換性は段階14で確認する。

## 確認した資料

Zephyrは**v4.4.2の公式ソース**を読み、変化する`latest`との差を避けた。版別HTMLは今回の取得手段では読めなかったため、タグ付きGitHubソースを根拠にした。外部資料とSlintのmasterは2026-10-10時点の確認であり、導入時に再確認する。

- Z1：[Zephyr v4.4.2リリース](https://github.com/zephyrproject-rtos/zephyr/releases/tag/v4.4.2)、[採用コミット](https://github.com/zephyrproject-rtos/zephyr/commit/dccb09599635bdff17633fa7e9dab014b91dce90)
- Z2：[FRDM-MCXN947](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/boards/nxp/frdm_mcxn947/doc/index.rst)
- Z3：[LCD_PAR_S035](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/boards/shields/lcd_par_s035/doc/index.rst)
- Z4：[OV7670・ピン配列](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/boards/shields/dvp_20pin_ov7670/doc/index.rst)
- Z5：[Video captureサンプル](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/samples/drivers/video/capture/README.rst)
- Z6：[SDK_VERSION](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/SDK_VERSION)
- Z7：[Getting Started](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/doc/develop/getting_started/index.rst)、[Python最低要件](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/cmake/modules/python.cmake)
- Z8：[Pythonの基本依存](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/scripts/requirements-base.txt)
- Z9：[ボードのrunner設定](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/boards/nxp/frdm_mcxn947/board.cmake)
- Z10：[LinkServer runner](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/scripts/west_commands/runners/linkserver.py)
- Z11：[west.yml](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/west.yml)
- Z12：[west manifestのfreeze仕様](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/doc/develop/west/manifest.rst)
- T1：[SDK 1.0.1・収録ツール・Windows配布物](https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v1.0.1)、[採用ZephyrのSDK導入オプション](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/scripts/west_commands/sdk.py)
- T2：[Python 3.12.10 Windowsインストーラ](https://www.python.org/downloads/release/python-31210/)
- T3：[west 1.5.0](https://pypi.org/project/west/1.5.0/)
- T4：[CMake 3.31.8](https://github.com/Kitware/CMake/releases/tag/v3.31.8)
- T5：[Ninja 1.13.1](https://github.com/ninja-build/ninja/releases/tag/v1.13.1)
- T6：[winget gperf 3.1 Windows配布情報](https://github.com/microsoft/winget-pkgs/blob/master/manifests/o/oss-winget/gperf/3.1/oss-winget.gperf.installer.yaml)
- T7：[Python 3.12.15・ソースのみの配布](https://www.python.org/downloads/release/python-31215/)
- T8：[oss-winget.dtc 1.6.1配布情報](https://github.com/microsoft/winget-pkgs/blob/master/manifests/o/oss-winget/dtc/1.6.1/oss-winget.dtc.installer.yaml)（段階1で追加）
- L1：PC内のNXP公式同梱資料`C:\nxp\LinkServer_1.4.85\ReleaseNotes.md`と`devices\FRDM-MCXN947.json`（対応版・デバイス定義を確認）
- L2：PC内のNXP公式同梱資料`C:\nxp\MCU-LINK_installer_3.133\Readme.txt`と`InfoProduct.txt`（更新パッケージの版を確認）
- S1：[Slint ZephyrデモREADME](https://github.com/slint-ui/slint/blob/master/demos/printerdemo/zephyr/README.md)（masterを参考として確認。採用版の固定は段階14）

## 変更履歴

| 日付 | 対象 | 変更前 → 変更後 | 理由 | 確認結果 |
|---|---|---|---|---|
| 2026-10-10 | 初期計画 | 記録用テンプレートを追加 | 段階0の準備 | 選定・実機確認は未実施 |
| 2026-10-10 | 段階0 | 未選定 → 本書の採用版・Windowsネイティブ方針 | PC実測と採用タグの対応を照合し再現条件を決定 | PC調査・資料確認・選定完了。導入・ビルド・実機確認は未実施 |
| 2026-10-10 | イシュー #1 | open → closed（completed） | 完了条件4項目を本記録とREADMEに照合し、すべて満たすことを確認 | GitHubの作業内容・完了条件をチェック済みに更新。段階1の実作業は未実施 |
| 2026-10-10 | 段階1の取得・DTC | SHAを渡すwest init → タグ取得＋SHA検証、SDK付属DTC 1.6.0 → oss-winget.dtc 1.6.1 | westの実際の引数仕様とWindows SDK配布物を照合した結果の訂正 | Zephyr SHA一致、DTC 1.6.1実行・アーカイブSHA-256照合成功。詳細は段階1の記録 |
| 2026-10-10 | 段階1 | 選定のみ → Codexによる環境構築・事前検証済み | イシュー #2の準備 | 有効67モジュールとPython169パッケージを固定。CodexがCOM5、MCU-Link V3.128、Hello World書き込み・リセット出力・main停止／ステップ／RAM確認成功を記録 |
| 2026-10-10 | イシュー #2 | closed → open（reopened） | ユーザー本人の実習前にクローズした判断を訂正 | 本人の完了条件を未チェックへ戻した。Codexの事前検証は本人の実習完了として扱わず、本人の完了確認とクローズの依頼または許可を得るまで閉じない |
