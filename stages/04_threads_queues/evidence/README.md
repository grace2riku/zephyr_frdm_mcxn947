# Codexによる段階4のビルド確認

実施日：2026-10-11（JST）。実施者：Codex。本人の実習結果ではない。

保存先は学習リポジトリの`.tmp/build/04_threads_queues_codex`。
本人用の`C:\Users\grace\zephyrproject-mcxn947\build\04_threads_queues`は使用しない。
ビルドは終了コード0で成功。生成設定とmapを確認した。
Codexによる書き込み・UART受信・実機確認は未実施。

## 実施コマンド

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
$threadsCheckBuild = Join-Path $learningRepo '.tmp\build\04_threads_queues_codex'
Set-Location $env:ZEPHYR_BASE
west build -b frdm_mcxn947/mcxn947/cpu0 -d $threadsCheckBuild "$learningRepo\stages\04_threads_queues"
$LASTEXITCODE
```

初回の保存先は未作成。制限付き実行でDTCが停止した後、同じソース・保存先を通常権限で再実行した。
成功ログは[build.log](build.log)、失敗ログは[build-restricted.log](build-restricted.log)。
PowerShellでログ保存には`2>&1 | Tee-Object -FilePath ...`を使用した。

## 結果

| 項目 | Codexによる確認結果 |
|---|---|
| 使用版 | Zephyr 4.4.2、Python 3.12.10、west 1.5.0、CMake 3.31.8、SDK 1.0.1、GCC 14.3.0、DTC 1.6.1 |
| ターゲット | frdm_mcxn947/mcxn947/cpu0 |
| 終了コード／コンパイル・リンク | 0／157/157まで成功。成功ログにwarning／errorの検出なし |
| FLASH | 28,868 B／全体2,097,152 B＝1.38% |
| RAM | 9,912 B／全体SRAM 524,288 B＝1.89%。CPU0の320 KiB領域に対しては3.02% |
| SRAMX | 0 B（領域96 KiB）。RAMとの重複なし。SRAM使用量合計は9,912 B |
| IDT_LIST | 0 B。RAM使用量へ加算する使用量なし |
| ELF／BINファイルサイズ | 775,376 B／28,868 B。ELFのファイルサイズはFlash使用量とは異なる |
| main.cのSHA-256 | 78D299680D01531C9BC9D7D41C463280AAE9FE5E8772F1BEAA59599C94625ACA |
| main設定 | CONFIG_MAIN_THREAD_PRIORITY=0、CONFIG_MAIN_STACK_SIZE=2048 |
| スレッド設定 | MULTITHREADING=y、NUM_PREEMPT_PRIORITIES=15。5と6は有効な範囲 |
| GPIO／表示設定 | GPIO、GPIO_MCUX、PRINTK、UART_CONSOLE、CBPRINTF_FULL_INTEGRALがy |
| tick | CONFIG_SYS_CLOCK_TICKS_PER_SEC=10000（1 tick＝0.1 ms） |
| LED定義 | led0→red_led、gpio0ピン10、Active Low（フラグ0x1）、red_ledとgpio0はokay |
| スタック | mapにcommand_stackとled_stackがあり、各0x800＝2048 B。要求サイズの静的配置を確認。実行時最大使用量は未測定 |
| ヒープ | CONFIG_HEAP_MEM_POOL_SIZE=0。アプリは動的メモリ確保を使わない。実行時の最大使用量測定は未実施 |

割合は使用量÷マイコン全体容量×100を小数点以下2桁で表示。
mapの領域配置と主要設定を[generated-settings.txt](generated-settings.txt)へ抜粋した。

## 問題と対処

制限付き実行ではDTCのMSYSランタイムのNtCreateDirectoryObjectがアクセス拒否（0xC0000022）となり、
設定処理が終了コード1で停止した。段階3と同じ種類のCodex側の実行制限。
ソースを変更せず通常権限で再実行するとDTC・設定生成・コンパイル・リンクまで成功した。
本人の端末での失敗としては扱わない。

ビルド成功は実機での指示反映や継続動作の確認を意味しない。
本人の実習結果は[段階4のREADME](../README.md#本人の実習結果)に記録する。
