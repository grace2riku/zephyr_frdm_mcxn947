# Codexによる段階5のビルド確認

実施日：2026-10-12（JST）。実施者：Codex。本人の実習結果ではない。

保存先は学習リポジトリの`.tmp/build/05_uart_tx_codex`。
本人用の`C:\Users\grace\zephyrproject-mcxn947\build\05_uart_tx`とは別。
ビルドは終了コード0、157/157のリンク完了で成功。生成設定・mapを照合した。
Codexによる書き込み・UART受信・LED観察は未実施。

## 実施コマンド

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
$uartCheckBuild = Join-Path $learningRepo '.tmp\build\05_uart_tx_codex'
$uartCheckCache = Join-Path $learningRepo '.tmp\cache\zephyr'
Set-Location $env:ZEPHYR_BASE
west build -b frdm_mcxn947/mcxn947/cpu0 -d $uartCheckBuild "$learningRepo\stages\05_uart_tx" -- "-DUSER_CACHE_DIR=$uartCheckCache"
$LASTEXITCODE
```

初回の保存先は未作成だった。ログはPowerShellの`2>&1 | Tee-Object -FilePath ...`で保存した。
制限付き実行はDTCのMSYSランタイムがアクセス拒否で停止。同じソース・保存先で通常権限のビルドを再実行すると成功した。
Codex用キャッシュもリポジトリの.tmp内へ指定した。これはビルド補助設定で、アプリの実行時の待機などを変更するものではない。

- [成功ログ](build.log)
- [制限付き実行の失敗ログ](build-restricted.log)
- [生成設定・mapとソースハッシュの抜粋](generated-settings.txt)

## 結果

| 項目 | Codexによる確認結果 |
|---|---|
| 使用版 | Zephyr 4.4.2、Python 3.12.10、west 1.5.0、CMake 3.31.8、SDK 1.0.1、GCC 14.3.0、DTC 1.6.1 |
| ターゲット／アプリ | frdm_mcxn947/mcxn947/cpu0／学習リポジトリのstages/05_uart_tx |
| 終了コード／リンク | 0／157/157成功。成功ログのwarning／error検出は0件 |
| FLASH | 27,452 B／全体2,097,152 B＝1.31% |
| RAM | 7,648 B／全体SRAM 524,288 B＝1.46%。CPU0の320 KiB領域に対して2.33% |
| SRAMX | 0 B（領域96 KiB）。RAMとの重複なし。SRAM合計使用量は7,648＋0＝7,648 B |
| IDT_LIST | 0 B。全体SRAMの使用量へ加算する量なし |
| ELF／BINのファイルサイズ | 733,360 B／27,452 B。ELFのファイルサイズはFlash使用量とは区別する |
| UART定義 | consoleとshell-uartはflexcomm4_lpuart4、compatibleはnxp,lpuart、status=okay、current-speed=0x1c200（115200）、pinctrl指定あり |
| UART経路 | SERIAL、UART_MCUX_LPUART、CONSOLE、UART_CONSOLE、PRINTKがy。LOGは無効。mapにmcux_lpuart_poll_outの実装を確認 |
| LED定義 | led0→red_led、GPIO0のピン0xa（10）、フラグ0x1（Active Low）。red_ledとgpio0はokay |
| main／追加スレッド | MAIN_THREAD_PRIORITY=6、MAIN_STACK_SIZE=2048、MULTITHREADING=y、NUM_PREEMPT_PRIORITIES=15。LED優先度5はソースで指定 |
| スタック | mapでled_stackとz_main_stackの各0x800（2048 B）の静的配置を確認。実行時最大使用量は未測定 |
| tick／ヒープ | SYS_CLOCK_TICKS_PER_SEC=10000、HEAP_MEM_POOL_SIZE=0。アプリは動的メモリ確保を使わない。CPU使用率・実行時の最大メモリ使用量は未測定 |
| main.cのSHA-256 | CDB98F63B17CDD577BF0AD63508E196230D0787ED485418F87DA6E78915F0D98 |

全体容量の使用率は使用量÷マイコン全体容量×100を小数点以下2桁で表示。
mapのRAMは0x30000000から320 KiB、SRAMXは0x14000000から96 KiBで、重複する領域ではない。
CPU0のRAM領域に対する2.33%と、全体SRAMに対する1.46%は分母が異なる。

## 問題と対処・確認範囲

制限付き実行ではDTCのNtCreateDirectoryObjectが0xC0000022で停止し、ビルドの終了コードは1だった。
段階4で記録したものと同じ種類のCodexの実行制限。ソースを変更せず通常権限で再実行すると成功した。
この失敗は本人のPowerShellで発生したものではなく、ソースのコンパイル失敗としても扱わない。

採用UARTドライバの送信可能状態のポーリングとレジスタ書き込みをローカルソースで照合した。
ビルド成功はPC側の受信や送信中のLED継続を確認した結果ではない。
本人のビルド・書き込み・実機確認は[段階5のREADME](../README.md#本人の実習結果)に別に記録する。
