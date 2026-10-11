# 段階1：環境構築とHello Worldのビルド・実行・デバッグ

対象：[イシュー #2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2)。
前提：[段階0](../00_versions/README.md)。Codexの事前検証日：2026-10-10（日本時間）。

## 目的と状態

westによるビルド、MCU-Link経由の書き込み、UARTの受信、GDBのソースレベル
デバッグをユーザー本人が操作して学ぶ。Codexは環境導入、クリーンビルド、
書き込み、リセット後のUART出力、main停止・ステップ実行・変数／メモリ確認を
事前検証し、結果を[evidence/](evidence/)へ保存した。
その後、ユーザー本人のビルド・書き込み・UART受信・GDBでのmain停止、
next／step、変数・メモリ確認、GDBの終了操作の成功を、本人が提供したログと報告で確認した。
2026-10-10にユーザー本人から「段階1の確認はOK」との確認を受けた。
その後、本人のPR確認と明示的な承認を経て[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)をmainへマージし、イシュー #2をcompletedとしてクローズ済み。
マージコミットは`0eb944e77a5baf0fcd31a91c9e2ac7a1430a6fc0`。

実習では、以下の手順を一つずつユーザー本人が実行し、Codexが日本語で操作の理由と
結果の見方を説明する。事前検証のログは参考として使い、ユーザー本人が観察した
結果を別に記録する。本人の完了確認とクローズの依頼または許可を得るまで、
イシューをクローズしない。

実習中の質問と回答は、実習結果とは別に[段階別のQA表](../../docs/qa.md)へ記録する。

`CMakeLists.txt`、`prj.conf`、`src/main.c`は採用Zephyrの公式Hello Worldと
同じ内容で、このフォルダを独立したアプリとしてビルドできる。
[取得元・ライセンス](UPSTREAM.md)も保持した。

## 機材・接続

- ボード：NXP FRDM-MCXN947、ターゲット：`frdm_mcxn947/mcxn947/cpu0`。
- データ通信対応USB Type-CケーブルでPCと**J17**を接続する。
- オンボードMCU-Link：CMSIS-DAP V3.128、シリアル`HN2APUBRDCEPT`。
- 実測UARTポート：**COM5**。別のPCやUSBポートでは再列挙する。
- UART：**115200 bps、8 data bits、no parity、1 stop bit、フロー制御なし**。
- CPU0、内蔵Flashを使用。LCD・カメラはこの段階では不要。

プローブの`r0E7`という表示を基板の印刷リビジョンとは扱わない。
基板の印刷リビジョンは未確認。COM番号と実装ファームウェアは
[シリアル情報](evidence/serial-port.txt)、[DAP情報](evidence/dapinfo.txt)で確認した。

## 使用版と保存先

| 項目 | 使用版／保存先 |
|---|---|
| ホスト | Windows 11ネイティブ、PowerShell、VS Code（段階0の既存環境） |
| Zephyr | v4.4.2、`dccb09599635bdff17633fa7e9dab014b91dce90` |
| SDK／GCC／GDB | 1.0.1 GNU Arm／14.3.0／16.2 |
| Python／west | CPython 3.12.10 x64／1.5.0、専用venv |
| CMake／Ninja／gperf／DTC | 3.31.8／1.13.1／3.1／1.6.1 |
| LinkServer | 既存1.4.85。MCU-Linkファームウェアは更新していない |
| 端末 | 手動操作は既存Tera Term 5.4.0、証跡取得はpyserial 3.5 |
| Zephyr＋依存モジュール | `C:\Users\grace\zephyrproject-mcxn947` |
| SDK | `C:\Users\grace\zephyr-sdk-1.0.1` |
| Python本体 | `C:\Users\grace\AppData\Local\Programs\Python\Python312` |
| venv | `C:\Users\grace\zephyrproject-mcxn947\.venv` |
| CMake等 | ワークスペースの`tools/`（版別フォルダ） |
| 通常ビルド | ワークスペースの`build/01_hello_world` |
| デバッグビルド | ワークスペースの`build/01_hello_world_debug` |

[実行版の記録](evidence/environment.txt)、[ホスト配布物SHA-256](evidence/host-tools-sha256.json)、
[SDK配布物SHA-256](evidence/sdk-sha256.json)を保存した。
SHA-256はPython／gperf／DTCのwinget manifestと、CMake／Ninja／SDKの
公式GitHub Releaseの公開値に照合した。PythonインストーラのAuthenticode署名も確認した。

### 段階0からの訂正

1. west 1.5.0の`init --mr`はタグ／ブランチ名を受け取るため、SHA直接指定は
   失敗した。`v4.4.2`を取得し、HEADと選定SHAの一致を検証する。
2. Windows SDKの実物にはDTCがないため、「SDK付属DTC 1.6.0」を
   `oss-winget.dtc 1.6.1`へ訂正した。採用Zephyrの公式Windows手順に従う。
3. SDKの`setup.cmd /h`は`Windows host tools are not available yet`と表示するが、
   minimal bundle内にOpenOCD・QEMU・wgetの配布物はある。DTCは別導入する。
4. 公式manifestは`babblesim`、`optional`、`testing`を既定で無効にする。
   `west update`は有効モジュールを取得するため、freezeには`--active-only`を使う。
   無効モジュールも対象にした当初コマンドは未取得モジュールで失敗した。

採用Zephyr、SDK、Python等の版はDTC以外変更していない。

## 環境構築：初回のみ

以下は**PowerShell 7**で実行する。既に構築済みのこのPCでは次節の
セッション設定から始める。外部ソース・SDK・venv・ビルド出力はGitへ追加しない。

### ホストツールとSDK

実行ディレクトリ：学習リポジトリ。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
Set-Location $learningRepo
& .\stages\01_hello_world\scripts\setup-host-tools.ps1
& .\stages\01_hello_world\scripts\setup-sdk.ps1
```

指定版の配布ファイルを取得し、ハッシュを検証してから導入する。
Pythonはユーザー用に併存導入し、CMake／Ninja／gperf／DTCは専用ワークスペースに
展開する。SDKスクリプトはminimal bundleとGNU Armだけを展開し、公式
`setup.cmd /t arm-zephyr-eabi /h /c`を実行する。全アーキテクチャを取得する必要はない。
この方式は採用版の`west sdk install --version 1.0.1 --gnu-toolchains arm-zephyr-eabi`
と同じ配布物を使用し、アーカイブをハッシュ証跡用に保持できる。

### venv、Zephyr、依存関係

初回は学習リポジトリから開始する。

```powershell
$zephyrWorkspace = 'C:\Users\grace\zephyrproject-mcxn947'
& "$env:LOCALAPPDATA\Programs\Python\Python312\python.exe" -m venv "$zephyrWorkspace\.venv"
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
python --version
python -m pip install 'west==1.5.0'
west init -m https://github.com/zephyrproject-rtos/zephyr --mr v4.4.2 -o=--depth=1 $zephyrWorkspace
if ($LASTEXITCODE -ne 0) { throw 'west init failed' }
if ((git -C "$zephyrWorkspace\zephyr" rev-parse HEAD) -ne `
    'dccb09599635bdff17633fa7e9dab014b91dce90') { throw 'Zephyr SHA mismatch' }
Set-Location $zephyrWorkspace
west update --narrow -o=--depth=1
west zephyr-export
'west==1.5.0' | Set-Content -Encoding ascii .\python-constraints.txt
west packages pip --install -- -c .\python-constraints.txt
python -m pip check
west --version
```

**各コマンドの終了コードが0であることを確認してから次へ進む。**
`--depth=1`はGit履歴の量を抑える。選定コミットと各モジュールの内容は変えない。
venvを分ける理由は既存Python環境へ依存パッケージを混在させないため。
`constraints`は依存導入によるwestの意図しない更新を防ぐ。
`zephyr-export`とSDKの`/c`はCMakeのユーザー用パッケージ登録を行う。

取得結果と解決結果を以下へ保存した。

- [west-frozen.yml](../00_versions/west-frozen.yml)：有効モジュールのimportを展開した固定SHA。
- [west-projects.txt](../00_versions/west-projects.txt)：実際のHEAD・manifest revision・配置先。
- [python-requirements-lock.txt](../00_versions/python-requirements-lock.txt)：`pip freeze --all`の結果。

```powershell
# 実行ディレクトリ：Zephyrワークスペース
west manifest --freeze --active-only -o "$learningRepo\stages\00_versions\west-frozen.yml"
west list -f '{name} {revision} {sha} {path}' |
    Set-Content -Encoding utf8 "$learningRepo\stages\00_versions\west-projects.txt"
python -m pip freeze --all |
    Set-Content -Encoding utf8 "$learningRepo\stages\00_versions\python-requirements-lock.txt"
```

再構築時はZephyr本体を上記SHAへ固定し、同じmanifestの既定グループで取得する。
frozen manifestの`self`はZephyr本体のSHAを固定しないため、本体SHAの併用が必要。
新しいvenvで`pip install -r <lockファイル>`し、`pip check`と各モジュールHEADを
保存記録に照合する。freezeはパッケージ版の固定で、wheel／sdistのハッシュ固定ではない。

## セッション設定：新しいPowerShellごとに実行

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
Set-Location $env:ZEPHYR_BASE
python --version
west --version
cmake --version
dtc --version
LinkServer -v
```

venvのScriptsと指定ツールをPATHの先頭に置き、`ZEPHYR_BASE`、
`ZEPHYR_TOOLCHAIN_VARIANT=zephyr`、`ZEPHYR_SDK_INSTALL_DIR`を明示する。
設定はこのPowerShellプロセスに適用する。既存のPython 3.10や
`arm-none-eabi`コンパイラを選んでしまうのを防げる。

## クリーンビルド

実行ディレクトリ：`$env:ZEPHYR_BASE`。まず公式サンプルをビルドする。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    -d "$zephyrWorkspace\build\01_hello_world_official" samples/hello_world
```

次に、この段階の独立アプリをビルドする。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    -d $helloBuild "$learningRepo\stages\01_hello_world"
```

`-p always`は古い設定キャッシュを捨てて作り直す。westがCMakeを呼び出し、
CMakeがボード・Kconfig・Devicetree・コンパイラからビルド設定を生成し、
Ninjaが実際のコンパイルとリンクを進める。
成果物はビルド先の`zephyr/zephyr.elf`、`zephyr.bin`、`zephyr.map`など。

## 書き込みとUART確認

J17へ接続し、ポートとプローブを列挙する。

```powershell
Get-CimInstance Win32_SerialPort | Select-Object DeviceID, Name
LinkServer probes --no-boot
LinkServer probe HN2APUBRDCEPT dapinfo
west -v flash -d $helloBuild -r linkserver --probe HN2APUBRDCEPT
```

`west flash`はボードに合うLinkServer引数とアドレスを渡す。LinkServer 1.4.85は
HEX未対応なのでrunnerがBINを選ぶ。MCU-LinkはSWDでFlashへ書き込む。

`west -v flash`の`-v`は詳細ログを有効にする。採用ZephyrのLinkServer runnerは、
LinkServer 1.4.85では通常モードの書き込み処理の標準出力を非表示にする。
そのため通常の`west flash`で終了コード0でも、Flashの進捗・成功表示は見えないことがある。
実習では詳細モードを使い、`Finished writing Flash successfully.`と終了コード0を確認する。

Tera TermでCOM5を開き、115200／8N1／フロー制御なしに設定してから
ボードの**RESET（SW1）**を押す。期待する行は次のとおり。

```text
Hello World! frdm_mcxn947/mcxn947/cpu0
```

このサンプルは起動時に一度だけ表示するので、書き込み後に端末を開いた場合は
再度リセットする。UARTの受信結果を保存する場合は、Tera Termを閉じてから
別PowerShellで次を実行し、受信中にリセットする。同時にCOM5を開けるアプリは1つ。

```powershell
python "$learningRepo\stages\01_hello_world\scripts\capture-uart.py" `
    --port COM5 --seconds 30 --output "$learningRepo\stages\01_hello_world\evidence\uart-reset.log"
```

自動検証では受信を開始してからLinkServerの`probe ... wiretimedreset 100`で
リセット線を100 msアサートする。SW1の手動操作自体は未検証。

## ソースレベルデバッグ

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    -d $helloDebugBuild "$learningRepo\stages\01_hello_world" -- '-DEXTRA_CONF_FILE=debug.conf'
west debug -d $helloDebugBuild -r linkserver --probe HN2APUBRDCEPT
```

`debug.conf`は`CONFIG_DEBUG_OPTIMIZATIONS=y`でソース行を追いやすい最適化を選び、
`CONFIG_DEBUG_THREAD_INFO=y`でスレッド情報を付ける。通常ビルドとは別の出力先を使う。
SDK付属GDBがELFのデバッグ情報を読み、LinkServerがSWD経由でCPUを停止・実行する。

GDBプロンプトで次を実行する。

起動時や長い出力で`--Type <RET> for more, q to quit, c to continue without paging--`
が表示されたら、Enterで次のページ、`c`＋Enterで残りをまとめて表示する。
`(gdb)`が表示されてから`set pagination off`で以降のページ送り待ちを無効にする。

```gdb
set pagination off
break main
monitor reset
continue
list
info locals
print z_main_thread.base.prio
print/x &z_main_thread
x/8wx &z_main_thread
info registers pc sp lr r0
next
step
step
bt
```

`next`はprintfの呼び出しを実行して次の行で止まり、`step`は関数へ入ることも
含めて1ステップ進む。`print`は型付きの変数を評価し、`x/8wx`は生のメモリを
32ビット値8個として読む。公式mainにはローカル変数がないため、
`info locals`の`No locals`は正常。実際の変数確認にはメインスレッドの優先度を使う。

停止状態からデバッグを終了するときは、GDBで以下を一つずつ実行する。
`monitor ondisconnect cont`は切断時にCPUを再開する設定、`detach`は実機から切断、
`quit`はGDBの終了。PowerShellの`PS ...>`へ戻ったら`$LASTEXITCODE`を確認する。

```gdb
monitor ondisconnect cont
detach
quit
```

同じ確認をログへ保存する手順：PowerShell Aで
`west debugserver -d $helloDebugBuild -r linkserver --probe HN2APUBRDCEPT`を起動し、
PowerShell Bでセッション設定を読んでから次を実行する。

```powershell
& "$env:ZEPHYR_SDK_INSTALL_DIR\gnu\arm-zephyr-eabi\bin\arm-zephyr-eabi-gdb.exe" `
    "$helloDebugBuild\zephyr\zephyr.elf" -batch `
    -x "$learningRepo\stages\01_hello_world\scripts\debug.gdb"
```

[debug.gdb](scripts/debug.gdb)はload、reset、main停止、変数・RAM・レジスタ確認、
next／stepを行い、detach時に実行を再開する。

`west debug`自体も次の形で実機検証した。runnerが接続とloadを済ませるため、
スクリプトのその部分を省いてコマンドを標準入力へ渡す。

```powershell
Get-Content -Encoding UTF8 "$learningRepo\stages\01_hello_world\scripts\debug.gdb" |
    Where-Object { $_ -notmatch '^(target remote |set mem inaccessible-by-default |load$)' } |
    west debug -d $helloDebugBuild -r linkserver --probe HN2APUBRDCEPT
```

## 学ぶAPI・設定

- `main()`：Zephyr初期化後、メインスレッドから呼ばれるアプリ入口。
- `printf()`：標準Cライブラリの出力。今回はボード既定のUARTコンソールへ出る。
- `CONFIG_BOARD_TARGET`：ビルドターゲット文字列を表示する。
- `prj.conf`：公式サンプルと同じ空設定。追加overlayは使わない。
- 生成`zephyr/.config`：実際に有効になったKconfigの値。
- 生成`zephyr/zephyr.dts`：ボードとSoC記述を展開したDevicetree。

## Codexによる事前検証結果

以下はCodexが実施した検証の記録で、ユーザー本人の実習完了を表さない。

| 確認 | 実測結果 | 証跡 |
|---|---|---|
| 公式サンプルのクリーンビルド | 成功。指定SDK・DTC・Pythonのパスを確認 | [build-official.log](evidence/build-official.log) |
| 段階フォルダのクリーンビルド | 成功。公式通常版とBINのSHA-256が一致 | [build-stage.log](evidence/build-stage.log)、[verification.txt](evidence/verification.txt) |
| デバッグビルド | 成功。debug.confの両設定が有効 | [build-debug.log](evidence/build-debug.log) |
| 書き込み | 公式版・段階フォルダ版とも成功。BINを`0x10000000`へ書き込み | [flash-official.log](evidence/flash-official.log)、[flash-stage.log](evidence/flash-stage.log) |
| リセットとUART | COM5／115200／8N1。リセット線操作後に期待するHello Worldを受信 | [uart-official-reset.log](evidence/uart-official-reset.log)、[uart-reset.log](evidence/uart-reset.log)、[reset-stage.log](evidence/reset-stage.log) |
| main停止 | `src/main.c:11`、printf実行前、PC=`0x100013a6` | [gdb-west.log](evidence/gdb-west.log) |
| next／step | nextでreturn行13、stepで`bg_thread_main`、さらに`z_thread_essential_clear`へ進む | [gdb-west.log](evidence/gdb-west.log) |
| 変数／メモリ | `z_main_thread.base.prio=0`、`&z_main_thread=0x30000238`、RAMを8ワード読み取り | [gdb-west.log](evidence/gdb-west.log) |
| 依存固定 | 有効67モジュールのHEADとfrozen SHAが一致、ソースツリーはすべて変更なし。無効17モジュールは対象外 | [verification.txt](evidence/verification.txt) |
| Python依存 | 169パッケージを固定、`pip check`成功 | [lock](../00_versions/python-requirements-lock.txt)、[verification.txt](evidence/verification.txt) |

UARTログは受信した生バイトを保持する。通常版の最終ログにはリセット中のNULを
1バイト含むが、リセット後のHello World行は正常に受信した。

| ビルド | Flash使用量／領域 | RAM使用量／領域 |
|---|---|---|
| 公式通常版・段階フォルダ通常版 | 26,596 B／2 MiB（1.27%） | 4,448 B／320 KiB（1.36%） |
| デバッグ版 | 32,132 B／2 MiB（1.53%） | 4,600 B／320 KiB（1.40%） |

これはリンク時の静的使用量で、実行時のスタック最大使用量ではない。
領域サイズは生成設定とリンカ出力による。SRAMX、IDT_LISTの使用量はどちらも0 B。
デバッグ後は通常版を再度書き込み、リセット後の出力を確認して終了した。

事前検証で未確認：基板の印刷リビジョン、Tera TermのGUI表示、手で押したSW1の動作。
CodexのUART確認はpyserial、リセットはMCU-Linkのリセット線操作で実施した。

## ユーザー本人の実習と完了条件

現在、ユーザーからJ17への接続済みという報告、公式Hello Worldおよび段階フォルダのクリーンビルド成功のログ、COM5とMCU-Linkの列挙結果を受け取っている。
詳細モードの書き込みログで成功表示と終了コード0を確認済み。リセット後の受信手順を案内し、Tera TermでZephyrの起動表示とHello Worldの受信結果を確認済み。デバッグ用ビルドと追加設定の反映、GDBでのmain停止、nextによる1行実行とUARTの追加出力、変数の値とメモリ確認は確認済み。stepでmainから戻る動作と関数内部へ入る動作、btで呼び出し関係、GDBの切断・終了と終了コード0も確認済み。
以下は本人の結果を確認してからチェックする。

- [x] 自分でクリーンビルドと書き込みを行い、成功を確認する。
- [x] 自分でリセット後のHello WorldをUARTで確認する。
- [x] 自分でmainに停止し、ステップ実行と変数／メモリ確認を行う。
- [x] READMEの使用版・手順・期待動作・本人の実習結果と未確認事項の記録を、本人が確認する。

実習結果は以下へ記録済み。2026-10-10の「段階1の確認はOK」との本人の回答をもって、最後の確認条件にもチェックした。
段階1終了時にはボードにデバッグ用ビルドが入っていた。本人の依頼で変更をコミット・pushし、PR #17を作成した。2026-10-10に本人がPRのソース・手順・実習記録を確認し、マージとクローズを承認。PR #17をmainへマージし、#2をcompletedとしてクローズした。PR提出時点の「#2は開いたまま」という記述は、その後の完了状態へ更新した。

本人の実習結果は、実習日、実行したコマンド、UARTの出力、デバッグで観察した
値や動作、疑問点と対処を記録する。

| 実習日 | 本人が実施した操作 | 提供されたログで確認した結果 |
|---|---|---|
| 2026-10-10 | セッション設定と公式Hello Worldのクリーンビルド | Python 3.12.10、west 1.5.0、Zephyr 4.4.2、SDK 1.0.1、CPU0向け。`west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d "$zephyrWorkspace\build\01_hello_world_official" samples/hello_world`の終了コード0。ELFとBINを生成。Flash 26,596 B、RAM 4,448 B。書き込み・実機出力はまだ未確認 |
| 2026-10-10 | 学習リポジトリの段階1アプリのクリーンビルド | `west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d $helloBuild "$learningRepo\stages\01_hello_world"`の終了コード0。`Application:`が段階フォルダを指すことを確認。ELF 720,796 B、BIN 26,596 Bを生成。Flash 26,596 B、RAM 4,448 B。書き込み・実機出力はまだ未確認 |
| 2026-10-10 | UARTポートとデバッグプローブの列挙 | `Get-CimInstance Win32_SerialPort`でCOM5（MCU-Link VCom Port）、`LinkServer probes --no-boot`でMCU-LINK FRDM-MCXN947、CMSIS-DAP V3.128、シリアル`HN2APUBRDCEPT`を確認。`r0E7`はプローブの表示であり、基板の印刷リビジョンは引き続き未確認 |
| 2026-10-10 | 通常モードで段階1アプリの書き込みコマンドを実行 | `west flash -d $helloBuild -r linkserver --probe HN2APUBRDCEPT`の終了コード0。補助処理の`UnicodeDecodeError`後もMCU-Linkの選択まで進んだ。runnerが詳細出力を非表示にするため、書き込み成功表示はログに含まれない。この時点では詳細モードとUARTの確認待ち |
| 2026-10-10 | 詳細モードで段階1アプリの書き込み結果を確認 | `west -v flash -d $helloBuild -r linkserver --probe HN2APUBRDCEPT`の終了コード0。`zephyr.bin`を`0x10000000`へ26,596 Bロードし、`Finished writing Flash successfully.`を確認。`Sectors written: 0, unchanged: 4`は既存Flashの内容が同じため書き換えを省略した表示。UART出力はまだ未確認 |
| 2026-10-10 | Tera TermでUART出力を確認 | ユーザー提供のTera Term表示で`*** Booting Zephyr OS build v4.4.2 ***`と`Hello World! frdm_mcxn947/mcxn947/cpu0`を確認。文字化けなく採用版とCPU0向けターゲット名を受信 |
| 2026-10-10 | デバッグ用アプリのクリーンビルドと設定確認 | `west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d $helloDebugBuild "$learningRepo\stages\01_hello_world" -- '-DEXTRA_CONF_FILE=debug.conf'`の終了コード0。最終`.config`の`CONFIG_DEBUG_OPTIMIZATIONS=y`と`CONFIG_DEBUG_THREAD_INFO=y`を確認。ELF 774,608 Bを生成。Flash 32,132 B、RAM 4,600 B。GDBでの実機デバッグはまだ未確認 |
| 2026-10-10 | GDBを起動し実機へ接続 | `west debug -d $helloDebugBuild -r linkserver --probe HN2APUBRDCEPT`でGDB 16.2がCPU0へ接続。デバッグELFのロード量32,132 Bと`(gdb)`プロンプトを確認。ページ送り待ちを解除し、`set pagination off`を実行。ロード前の初回resetエラーは発生したが、その後のロードは完了。mainでの停止はまだ未確認 |
| 2026-10-10 | mainにブレークポイントを設定して停止 | `break main`で`0x100013a6`、`src/main.c:11`にハードウェアブレークポイントを設定。`monitor reset`で`Processor connection reset`を確認し、`continue`後に`Thread 2 hit Breakpoint 1, main ()`で停止。printf実行前の11行目を確認 |
| 2026-10-10 | ソース表示とnextによる1行実行 | `list`でmainのソースを表示。`next`で11行目のprintfを実行し、13行目の`return 0;`の直前に停止したことを確認。Tera TermにHello Worldの行が追加されたこともユーザー本人の報告で確認 |
| 2026-10-10 | GDBでローカル変数とメインスレッドの優先度を確認 | `info locals`の`No locals.`はmainにローカル変数がないため正常。`print z_main_thread.base.prio`で`$1 = 0 '\000'`を確認し、優先度の値0を読み取った |
| 2026-10-10 | GDBで構造体のアドレスとメモリを確認 | `print/x &z_main_thread`で`0x30000238`を確認。`x/8wx &z_main_thread`で先頭32 Bを読み取り、`0x30000238`と16 B先の`0x30000248`に4ワードずつ表示。8ワードは`0x30000498 0x30000498 0x00000000 0x00000001 0x00000080 0x00000000 0x00000000 0x00000000` |
| 2026-10-10 | stepでmainから戻り呼び出し元を確認 | `step`で`main`のreturnから`bg_thread_main`の`kernel/init.c:351`へ戻った。`bt`の`#0 bg_thread_main`と`#1 z_thread_entry`で現在位置と呼び出し元を確認。停止行は`z_thread_essential_clear(&z_main_thread);` |
| 2026-10-10 | stepで呼び出し先の関数内部へ入る | もう一度`step`を実行し、`z_thread_essential_clear`の`kernel/include/kthread.h:210`へ進んだ。引数`thread=0x30000238 <z_main_thread>`を確認。`bt`で`#0 z_thread_essential_clear`、`#1 bg_thread_main`、`#2 z_thread_entry`の呼び出し関係を確認 |
| 2026-10-10 | CPUの再開設定とGDBの終了 | `monitor ondisconnect cont`、`detach`、`quit`を実行。`Ending remote debugging.`、CPU0からの切断、GDB serverの終了、PowerShellへの復帰を確認。`$LASTEXITCODE`は0。ボード上のプログラムはデバッグ用ビルドのまま |

## トラブルと対処

- `west init --mr <SHA>`が失敗：上記のタグ取得＋SHA一致確認へ修正した。
  失敗後の空の`.west`のみを確認して除去し、再実行した。
- `west manifest --freeze`が未取得`canopennode`で失敗：公式既定の無効グループを
  含んでいたため、`--active-only`へ修正した。
- LinkServerの`probes`で`UnicodeDecodeError`：同梱USB列挙VBSの日本語出力を
  UTF-8として読む処理で発生した。MCU-Link用の確認では`probes --no-boot`で
  LPC-Link用ブート処理を省略すると列挙成功。コードページ変更だけでは解消しなかった。
  `west flash/debug`のログにも補助処理のエラーは出るが、MCU-Link選択後の
  書き込みとGDB確認は成功した。既存LinkServerのこの問題は残る。
- 初回GDB接続のresetが`No image address available for soft reset`と表示：
  LinkServerがELFをloadする前は初期アドレスを持たないため。
  load後の`monitor reset`は成功した。保存したdebug.gdbはload→resetの順にした。
  `west debug`内部のload前resetには同じ表示が残るが、その後のload・reset・main停止は成功。
- 未引用の`-DEXTRA_CONF_FILE=debug.conf`が`debug`と`.conf`に分割された：
  PowerShellでは引数全体を引用して修正した。
  [初回エラーログ](evidence/build-debug-initial-error.log)を保持した。
- 制限付きシェルでLinkServer起動がDLL再配置エラーになった：通常権限の実行では
  起動成功。ボードの故障とは区別する。

## 公式資料

- [採用版Getting Started](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/doc/develop/getting_started/index.rst)
- [採用版FRDM-MCXN947](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/boards/nxp/frdm_mcxn947/doc/index.rst)
- [採用版LinkServer runner](https://github.com/zephyrproject-rtos/zephyr/blob/v4.4.2/scripts/west_commands/runners/linkserver.py)
- [SDK 1.0.1配布物](https://github.com/zephyrproject-rtos/sdk-ng/releases/tag/v1.0.1)
- [DTC 1.6.1配布情報](https://github.com/microsoft/winget-pkgs/blob/master/manifests/o/oss-winget/dtc/1.6.1/oss-winget.dtc.installer.yaml)
