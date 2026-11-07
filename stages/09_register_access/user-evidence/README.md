# 段階9：本人の実習記録

2026-11-01（JST）に開始。本人のファイル照合・ビルド・Flash書き込み成功は2026-11-07に確認済み。
UART起動・プロンプト・初期LED消灯・helpに加え、reg infoと3レジスタの初期読み出しも確認済み。
PCOR・PSOR・PTORの書込、LED変化とPDOR、消灯状態での3操作先への値0による状態維持も確認済み。
不正入力9件の拒否・消灯維持・正常コマンドの応答も確認済み。GDB照合は2026-11-07の本人の指示で省略した。結果は未受領で、実施済みとは扱わない。
本人からRMをNXPから取得できるとの回答後、PDFの提供を受領（2026-11-01）。
実物はRev. 7, 2025-10-10、3,763ページ。Codexによる[本文照合](../REGISTER_REFERENCE.md)は完了した。
本人によるregister_access.h/.cのソース確認は下記の報告で確認済み。
マニュアル本文の読解と後続のソース理解完了の報告は未受領。本人のビルド・実機結果は下記へ記録した。

## 本人の環境確認

受領日：2026-11-01（JST）。本人提供のPowerShell表示に基づく。実行の正確な時刻は未取得。

| 本人が表示した項目 | 提供表示の値 | 判定 |
|---|---|---|
| ZEPHYR_BASE | `C:\Users\grace\zephyrproject-mcxn947\zephyr` | 期待値と一致 |
| west --version | `West version: v1.5.0` | 一致 |
| Zephyrのgit rev-parse HEAD | `dccb09599635bdff17633fa7e9dab014b91dce90` | 一致 |
| 学習リポジトリの作業ブランチ | `codex/stage9-register-access` | 一致 |

Codexの事前検証とは別の本人の確認結果として記録する。
環境確認後にregister_access.h/.cの説明を案内した。後日の本人の確認結果は下記へ記録する。

## 技術の目的と用語の質問

2026-11-07（JST）に、使用技術の目的が分からないことと、「MMIOを実行」「許可表の検査」の意味について質問を受領した。
UART・NT-Shell・GPIO API・MMIO・Devicetree・BUILD_ASSERTの担当を整理し、点灯コマンドを例に、
入力→操作条件の検査→レジスタ書込み→端子出力→LEDという流れを説明する。
回答と図は[QA-051〜053](../../../docs/qa.md#qa-051-supplement)へ記録した。
質問への回答時点では本人のソース確認・理解完了の報告は未受領だった。

## MMIO・許可表の理解とregister_access.h/.cの確認

受領日：2026-11-07（JST）。本人から次の報告を受領した。

> MMIOと許可表の検査の意味がわかりました。register_access.h/.cのコードも確認できたので、次のregister_commands.cの説明に進めてください。

本人の理解・ソース確認完了の報告として記録する。本人による再説明の確認や実機確認とは区別する。
依頼に基づきregister_commands.cの説明へ進める。同ファイルは、入力の単語列を受け取り、許可表・数値・幅・
書込ビットを検査してからregister_access_read／writeを呼び、書込結果をPDOR bit 10で検証する担当。
許可表の構造、usage／info、parse_number、argc／argv、MMIO前の検査、書込前後の確認を順に説明する。
その説明直後の本人報告と、先へ進める依頼は次節に記録する。

## register_commands.cの理解保留と次の手順

受領日：2026-11-07（JST）。本人から次の報告・依頼を受領した。

> 理解はできていませんが、先にすすめてください。

register_commands.cは未理解と記録し、理解・コード確認完了に更新しない。
本人の依頼に基づき、理解確認を保留して次へ進める。register_access.h/.cの以前の確認結果は保持する。
commands.cの入力検査・単語分割・振り分け、main.cの起動順、CMakeLists.txt・prj.confの役割を短く説明し、
NT-Shellのチェックサム照合とCPU0向けビルドを本人へ案内する。
この案内時点では、後続ソース・設定の本人の理解／確認完了、外部OSS照合・ビルド結果は未受領だった。
ビルド成功の報告を受けても、ソース理解完了としては扱わない。

## 本人のファイル照合とビルド

受領日：2026-11-07（JST）。本人から「ファイル照合はPASSで終了コード0、ビルドも終了コード0で成功」との報告と、
リンク・使用量・生成先・終了コードの表示抜粋を受領した。実行の正確な時刻は未取得。

- ファイル照合：案内したverify_vendor.pyに対する本人のPASS・終了コード0の報告。出力全文は未受領。
  今回の案内はチェックサム照合であり、本人による公式アーカイブ同時照合の結果とは扱わない。
- ビルド：166/166 Linking C executable zephyr/zephyr.elf、終了コード0。
- 生成先：`C:/Users/grace/zephyrproject-mcxn947/build/09_register_access/zephyr/zephyr.elf`。
- ボード：`frdm_mcxn947/mcxn947/cpu0`。

| 対象 | 提供ビルド表示の使用量 | マイコン全体の容量 | 全体に対する使用率 |
|---|---:|---:|---:|
| Flash | 34,436 B | 2,097,152 B | 1.64% |
| SRAM | 8,744 B | 524,288 B | 1.67% |

提供表示のRAM領域は320 KiB、同領域に対する使用率は2.67%。SRAMX 0 B／96 KiB、IDT_LIST 0 B／32 KiB。
Codexの別保存先の事前ビルドとは別の本人実行結果として記録し、今回は同じ使用量だった。
ソース理解完了やボードでの実行結果を表すものではない。

### 本人用生成物をCodexが読み取り照合した結果

実施日：2026-11-07（JST）。本人用の上記ビルド先を読み取り、次を確認した。
本人が生成設定を表示して確認した結果とは区別する。追加ビルド・ボード接続・書き込みは行っていない。

- CMakeCache：段階9アプリ・prj.conf・CPU0・採用Zephyrパス・SDK 1.0.1のコンパイラに一致。
- .config：SERIAL／UART_INTERRUPT_DRIVEN／UART_MCUX_LPUART／GPIO／GPIO_MCUX／ARM_SECURE_FIRMWARE有効。
  CONSOLE／CONSOLE_SUBSYS／SHELL／PRINTK／LOG／BOOT_BANNER／UART_ASYNC_API／LED／INPUT無効。
  main優先度6、mainスタック4096 B。
- Devicetree生成ヘッダー：GPIO0ベース0x50096000、led0はgpio0・pin 10・flags 1（Active Low）。
  zephyr,consoleはflexcomm_b4000配下のlpuart_b4000。
- ELFのnm：_flash_used=0x8684、_image_ram_size=0x2228。memory_test_bufferは0x30000670・0x80 B。
  register_access_*のシンボルあり、LEDのstack／entryシンボルなし。
- ELFのobjdump：register_access_writeはGPIO0ベース＋offsetへの単一32 bit str。read-modify-writeなし。
- runners.yaml：LinkServer、MCXN947:FRDM-MCXN947、cm33_core0、Flash 0x10000000。

本人による今回のCOM・プローブ列挙、書き込み、UART起動・初期LEDの結果は後続節へ記録した。

## 本人による今回の接続先確認

受領日：2026-11-07（JST）。本人がGet-CimInstance Win32_SerialPortとLinkServer probes --no-bootを実行した表示を受領した。
実行の正確な時刻は未取得。前段階の接続先情報を今回の結果として流用しない。

| 項目 | 今回の提供表示 |
|---|---|
| COM | COM5 |
| シリアルポート名 | MCU-Link VCom Port (COM5) |
| プローブ番号 | 1 |
| プローブ | MCU-LINK FRDM-MCXN947 (r0E7) |
| ファームウェア表記 | CMSIS-DAP V3.128 |
| プローブのシリアル | HN2APUBRDCEPT |

Codexが本人用runners.yamlのLinkServer・MCXN947:FRDM-MCXN947・cm33_core0・Flash 0x10000000と、
採用runnerの--probe指定を読み取り再確認した。本人の接続先確認とは実施者を分けて記録する。
同じ本人ビルド先とシリアルHN2APUBRDCEPTを使用するwest -v flashの実行を案内した。
その結果は次節へ記録する。

## 本人のFlash書き込み

受領日：2026-11-07（JST）。本人からFlash成功表示・ロード量・終了コードを含む表示抜粋を受領した。
実行の正確な時刻は未取得。案内した本人用ビルド先・確認済みプローブを使用する手順に対する報告として記録する。
提供抜粋にはビルド先やプローブの選択行自体は含まれていない。

- `Finished writing Flash successfully.`を確認。
- `Loaded 0x8684 bytes in 295ms (about 116kB/s)`を確認。
  0x8684は34,436 Bで、本人ビルド表示・ELFの_flash_usedと本人用zephyr.binのファイル長に一致する。
- `$LASTEXITCODE`の表示は0。
- system reset、stall addressでの実行開始、0x50000040のboot ROM stall用watchpointへの切り替えの表示も含まれる。
  これらは書き込み後のツール表示として保存し、アプリのUART起動やLEDの確認済みとは扱わない。

チャット提供抜粋を[flash-excerpt.log](flash-excerpt.log)へ転記した。完全なログファイルや元の改行バイト列の保存ではない。
次はTera TermのCOM5・115200・8N1・フロー制御なし・ローカルエコーOFF・送信CR+LF／受信CRを案内し、
受信準備後のRESETによる段階9の起動表示・プロンプト・赤色LED消灯とhelp応答を本人が確認する。
その後の本人の起動・LED・help確認報告は次節へ記録する。

## 本人の起動・初期LED・help確認

受領日：2026-11-07（JST）。本人から次の報告を受領した。

> 起動表示とmcxn947>を確認できました。赤色LEDは消灯しています。helpでも使い方が表示され、プロンプトへ戻りました。

案内した段階9の起動表示・プロンプト、赤色LEDの消灯、helpによる使い方表示とプロンプト復帰を、本人の確認報告に基づき記録する。
UART出力の原文、端末設定画面の実値、GPIOのアドレス一覧・RAM情報の実値そのものは未受領。
実際の操作の正確な日時は未取得。RESETの個別の実施報告とは区別する。
次はreg infoとPDOR／PDDR／PDIRの32 bit読み出しを1行ずつ案内し、UARTの値とLED消灯の継続を確認する。
その後に受領した読み出し・PCOR／PSORの書き込み結果は後続節へ記録する。

## 本人の許可表表示と初期レジスタ読み出し

受領日：2026-11-07（JST）。本人からreg infoと3つのreadのUART表示、
PDOR／PDDRのbit 10は1で赤色LEDは消灯のままとの報告を受領した。

| 操作 | 提供UARTの結果 | 確認した内容 |
|---|---|---|
| reg info | GPIO0・mask 0x00000400、PDOR 0x50096040／PSOR 0x50096044／PCOR 0x50096048／PTOR 0x5009604c／PDIR 0x50096050／PDDR 0x50096054 | ソース・本人用生成物のベース＋offsetと一致。read／writeの許可方向も一致 |
| PDORの32 bit read | 0x00000400 | bit 10が1。起動後の出力ラッチHighに一致 |
| PDDRの32 bit read | 0x00000400 | bit 10が1。出力方向に設定されている |
| PDIRの32 bit read | 0x00000449 | bit 10が1。他の入力ビットを含み、PDOR全体との一致は要求しない |
| 赤色LED | 消灯のままとの本人報告 | 初期状態のまま読み出しを実行できたとの目視報告 |

チャット提供表示を[register-read-excerpt.log](register-read-excerpt.log)へ転記した。
元の端末ログの改行バイト列を保持したファイルではない。実行の正確な日時は未取得。
Shellでの初期読み出しの確認であり、GDBによる照合はまだ未実施。

次はPCORへ0x400を書いて点灯しPDORをread、同じPCOR書き込みとreadで点灯維持を確認、
PSORへ0x400を書いて消灯しPDORをreadする手順を案内する。1行ごとの応答を待ち、UARTと目視を両方記録する。
その後に受領した結果は次節へ記録する。

## 本人のPCOR・PSOR書き込みとLED・PDORの確認

受領日：2026-11-07（JST）。本人からPCORを2回、PSORを1回書き込み、すべてOK応答だったとの報告と、
各操作直後のPDOR readを含むUART表示を受領した。

| 操作 | 提供UARTの書込応答／PDOR read | 本人のLED目視報告 |
|---|---|---|
| PCOR 0x50096048へ0x400、32 bit | OK、PDOR.bit10=0／0x00000000 | 点灯 |
| 同じPCORへ0x400を再度書込 | OK、PDOR.bit10=0／0x00000000 | 点灯維持 |
| PSOR 0x50096044へ0x400、32 bit | OK、PDOR.bit10=1／0x00000400 | 消灯 |

PCORの反復で状態を維持することと、PSORで出力をHighへ戻すことを、
UARTの出力ラッチ値と別途のLED目視報告で確認した。最後の状態はPDOR=0x00000400、赤色LED消灯。
チャット提供表示は[register-set-clear-excerpt.log](register-set-clear-excerpt.log)へ転記した。
元の端末ログの改行バイト列を保持したファイルではない。操作の正確な日時は未取得。
割り込みステータスW1Cのクリア試験やGDBとの照合を行った結果ではない。

次はPTORへ0x400を書いてPDOR readを2回行い、点灯→消灯を確認する。
続いてPSOR／PCOR／PTORのそれぞれへ値0を書き、各書込直後のPDOR readと目視で消灯維持を確認する。
その後に受領した結果は次節へ記録する。

## 本人のPTOR反転と値0による状態維持

受領日：2026-11-07（JST）。本人からPTORの2回の書き込みと、3書込先への値0の書き込みについて、
すべてOKとの報告・各書込直後のPDOR readを含むUART表示・LEDの目視報告を受領した。

| 操作 | 提供UARTの書込応答／PDOR read | 本人のLED目視報告 |
|---|---|---|
| PTORへ0x400、32 bit、1回目 | OK、PDOR.bit10=0／0x00000000 | 点灯 |
| 同じPTORへ0x400、2回目 | OK、PDOR.bit10=1／0x00000400 | 消灯 |
| PSORへ0、32 bit | OK、PDOR.bit10=1／0x00000400 | 消灯のまま |
| PCORへ0、32 bit | OK、PDOR.bit10=1／0x00000400 | 消灯のまま |
| PTORへ0、32 bit | OK、PDOR.bit10=1／0x00000400 | 消灯のまま |

PTORの反転2回で元の消灯状態へ戻り、その後の3書込先への値0は消灯状態を維持した。
値0の実機確認は消灯状態で実施した範囲で記録し、点灯状態での値0の実施結果へ広げない。
最後の状態はPDOR=0x00000400、赤色LED消灯。
チャット提供表示を[register-toggle-zero-excerpt.log](register-toggle-zero-excerpt.log)へ転記した。
元の端末ログの改行バイト列を保持したファイルではない。実行の正確な日時は未取得。

次は不整列・許可外アドレス／操作方向・許可外ビット・幅・符号・桁あふれを意図的に入力し、
ERROR・プロンプト復帰とLED消灯維持を確認する。最後に正常なPDOR readとhelpの応答を確認する。
その後に受領した不正入力の結果は次節へ記録する。GDB照合の結果はまだ未受領。

## 本人の不正入力9件と正常コマンドへの復帰

受領日：2026-11-07（JST）。本人から9件すべてERROR・プロンプト復帰、LED消灯維持、
最後のreadはPDOR=0x00000400、helpも正常表示だったとの報告と、9件のERRORのUART表示を受領した。

| 不正入力 | 提供UARTで確認した拒否 |
|---|---|
| reg read 0x50096041 32 | address or access not permitted。不整列・表にないアドレス |
| reg read 0x50096120 32 | address or access not permitted。許可表にないISFR |
| reg read 0x50096044 32 | address or access not permitted。PSORのShell readは禁止 |
| reg write 0x50096050 32 0 | address or access not permitted。PDIRのwriteは禁止 |
| reg write 0x50096040 32 0x400 | address or access not permitted。PDORのShell writeは禁止 |
| reg write 0x50096048 32 0x401 | write value contains bits outside 0x00000400 |
| reg read 0x50096040 16 | invalid register command, number or widthとUsage。幅32以外 |
| reg write 0x50096048 32 -1 | invalid register command, number or widthとUsage。符号付き値 |
| reg write 0x50096048 32 0x100000000 | invalid register command, number or widthとUsage。32 bit桁あふれ |

最後の正常read・helpの出力全文は未受領のため、PDOR=0x00000400・help正常応答は本人の報告に基づく確認として記録する。
LED消灯維持と各ERROR後のプロンプト復帰も本人報告で確認した。
提供ERROR表示は[register-invalid-excerpt.log](register-invalid-excerpt.log)へ転記した。
元の端末ログの改行バイト列を保持したファイルではない。実行の正確な日時は未取得。
最後の報告状態は赤色LED消灯、PDOR bit 10=1。理解保留の状態は変更しない。

次は同じ本人用ELFでGDBとUARTの値を照合する。Codexが採用LinkServer runnerを読み取り、
west debugは指定ELFで接続後、初期monitor resetとloadを行うこと、attachとは処理が異なることを確認した。
今回確認済みプローブHN2APUBRDCEPTを指定するwest debugと、(gdb)到達後のset pagination offを案内する。
ページ送りのType <RET> for moreが出た場合はEnterで進める。Tera Termは開いたままにし、CPU停止中はShellが応答しないことを説明する。
GDB接続・ELFロード・実行再開・消灯／点灯時の照合はまだ未実施。

今後、コード確認、RMの読解、ビルド使用量、COM／プローブ、書き込み結果、
UARTとLED、拒否後の復帰、GDB照合、最終状態を実施者・受領日・根拠とともに記録する。
Codexの事前検証を本人の結果へ転記しない。期待表示と実測ログを区別する。

## 学習方針の見直しとGDB照合の省略

受領日：2026-11-07（JST）。本人から、GDBでのレジスタ値照合の意義・目的が分からないため今回は省略し、
このリポジトリの目的であるFRDM-MCXN947でのZephyr学習に注力するよう依頼を受領した。
さらに、次の方針を段階9に限らず今後も継続する意図を明示した。

> Zephyrの設定からデバイスを使う流れ、ドライバAPI、スレッド・キューによる役割分担
> 各段階で何のZephyr機能を学ぶかを明記

GDB照合は今回の作業内容・完了条件から除外し、未実施を完了に変更しない。
前節までのGDB案内は変更前の計画として残す。以後はこの本人の指示を優先し、GDB起動や照合を進行条件にしない。
最後に受領した実機状態はLED消灯・PDOR=0x00000400・help正常応答。新しいボード操作は案内しない。
register_commands.cの理解保留は維持し、実機成功をコード理解完了に読み替えない。

README・ロードマップ・AGENTS.mdと段階9〜15の学習イシュー（#10〜#16）へ、Zephyrの3つの学習の軸と各段階の目的を反映した。
GitHubの7イシューは本文・題名を更新し、いずれもOPENを維持した。マージ・クローズは行っていない。
段階9の追加実験を増やさず、既に確認した実機結果の記録と本人のREADME確認へ進める。
Slintは任意の発展課題として位置付ける。PR確認・マージ・イシュークローズは未実施。

## 段階9の残る確認と終了手順の案内

受領日：2026-11-08（JST）。本人がこの方針で進めることを確認し、段階9を終えるための残りの確認と次の手順を依頼した。
学習方針への賛同として記録し、README内容の本人確認完了や、PRのマージ・イシュークローズの承認とは解釈しない。

Codexがイシュー #10の現在の本文・OPEN状態とPR検索を確認した。
見直し後の未完了チェックは、実習記録が実際の学習・実習と一致するかの本人確認。
codex/stage9-register-accessのPRは検索結果に存在せず、ローカル変更は未コミット。
新しい実機操作や、保留したコードの理解確認を追加しない。

READMEの「実習記録の本人確認」に、目的・実施済み・省略・理解保留・使用量・最後に受領した状態の一覧を用意した。
本人の一致報告後に確認日を記録し、コミット・プッシュ・PR作成、本人のPR確認、明示的な承認後のマージ・クローズの順に進める手順を案内する。
READMEの一致報告はこの時点では未受領。追加のビルド・ボード操作は実施していない。

## READMEの6項目の本人確認とPR作成依頼

受領日：2026-11-08（JST）。本人から次の確認・依頼を受領した。

> READMEの6項目を確認しました。実際の実習内容と一致しています。コミット・プッシュして、段階9のPRを作成してください。

目的・実施済み・省略／対象外・理解保留・使用量・最後に受領した実機状態の6項目について、
本人が実習内容との一致を確認したと記録する。README本人確認は完了。
コミット・プッシュ・PR作成の依頼に基づき、段階9の実装・証跡・Zephyr学習の継続方針をPRへまとめる。
GDBの省略とregister_commands.cの詳細理解保留は保持する。追加ビルド・ボード操作は行わない。
PRの本人確認・マージ・イシュー #10のクローズは、この依頼時点では未実施・未承認。

## コミット・プッシュとPR作成

実施日：2026-11-08（JST）。本人の明示的な依頼に基づきCodexが実施した。

- 実装・証跡・学習方針のコミット：`4d7a0ed0515a70fd3276b7e4ff223507647b265c`。
- 作業ブランチ：`codex/stage9-register-access`。originへのpushと追跡設定に成功。
- 成果物：[PR #27](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/27)、base main、OPEN、draftではない。
- PR作成前に、ステージした50ファイルの範囲、NT-Shell 16ファイルのSHA-256とバイト列、
  ログ転記のバイト列、文書のローカルリンク、git diff --cached --checkを確認した。
- ソースはビルド・実機確認後に変更していないため、成功済みのCPU0ビルド・QEMU 9テストは繰り返していない。
  追加ビルド・ボード操作は行っていない。

PR作成後にリンクと状態の記録を追加した。本人のPR確認・マージ・イシュー #10のクローズは未実施。
GDB照合の省略とregister_commands.cの詳細理解保留は変更しない。
