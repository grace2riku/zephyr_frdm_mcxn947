# 段階6：Codexによる事前確認

実施日：2026-10-24（JST）。本人のビルド・書き込み・実機確認とは区別する。
実機へのアクセス・書き込み・端末操作は行っていない。

## 割り込み版のビルド

- 採用Zephyr：4.4.2、`dccb09599635bdff17633fa7e9dab014b91dce90`。
- ターゲット：`frdm_mcxn947/mcxn947/cpu0`。
- CMake 3.31.8、Python 3.12.10、west 1.5.0、SDK 1.0.1、GCC 14.3.0、DTC 1.6.1を表示で確認。
- アプリ入力：`stages/06_uart_echo`、`ECHO_MODE=irq`、`prj_irq.conf`。
- 出力：リポジトリ内の`.tmp/build/06_uart_echo_irq_codex`（Git管理外）。
- 154/154のリンク完了、終了コード0。

ZEPHYR_BASEから実行した最初のコマンド：

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    "$learningRepo\stages\06_uart_echo" -d $stageIrqCheckBuild -- -DECHO_MODE=irq
```

制限実行ではDTCのMSYS共有オブジェクト作成がアクセス拒否（NtCreateDirectoryObject、0xC0000022）で失敗した。
通常権限で同じ入力・出力先を指定し、`-p always`を付けず再実行すると成功した。
この失敗を本人の環境の不具合やアプリのコンパイルエラーとは扱わない。

## 生成物の確認

- CMakeCache：ECHO_MODE=irq、prj_irq.conf、CPU0ボード、採用Zephyrの場所を確認。
- .config：SERIAL、UART_INTERRUPT_DRIVEN、UART_MCUX_LPUART、UART_MCUX_LPUART_ISR_SUPPORT、GPIO、GPIO_MCUXがy。
- .config：CONSOLE、CONSOLE_SUBSYS、SHELL、PRINTK、LOG、UART_ASYNC_APIは無効。
- main優先度6、スタック要求サイズ2048 B。LED優先度5、要求サイズ2048 Bはソースで確認。
- Devicetree：consoleはflexcomm4_lpuart4、nxp,lpuart、115200、okay。
- Devicetree：led0はred_led、GPIO0のピン10、Active Low、okay。
- map：rx_lineは40 B、メッセージキューは40 Bを10件保持する構成。キュー制御構造などは別。
- 受信はISRだけ、UART出力はmainだけ。ISRはキュー投入でK_NO_WAITを指定し、sleep・UART送信を呼ばないことをソースで確認。

Flash 25,136 B／2,097,152 B＝1.20%。SRAMはRAM 8,120 B＋SRAMX 0 B＝8,120 B／524,288 B＝1.55%。
IDT_LISTは0 B。CPU0の320 KiB領域に対しては2.48%。

## 未確認

本人の割り込み版ビルドは、その後終了コード0の報告を受領し、段階6のREADMEへ別に記録した。
本人の割り込み版の基本実機動作（入力なしでのLED継続、31/32 B、CR/LF各設定）は、その後確認済み。
結果は段階6のREADMEに別記した。本人の3行・100行連続入力と最後のokも、その後確認済み。
その100行試験では満杯通知は観測されず、キュー満杯分岐の実機確認は未実施。
ビルド成功のみでは実機の完了条件を満たしたと扱わない。
