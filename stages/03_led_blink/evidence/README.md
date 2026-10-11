# Codexによる段階3のビルド確認

実施日：2026-10-11（JST）。実施者：Codex。**本人の実習結果ではない。**

`BLINK_INTERVAL_MS=500`と`250`の両版をCPU0向けにビルドし、生成設定を確認した。
Codexによるボードへの書き込み、UART受信、LEDの実機確認は未実施。
本人用の`C:\Users\grace\zephyrproject-mcxn947\build\03_led_blink`は使用していない。

## 500 ms版

以下は500 ms版の確認時（ソースの定数が500だった時点）のコマンドと結果。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
$blinkCheckBuild = Join-Path $learningRepo '.tmp\build\03_led_blink_codex'
Set-Location $env:ZEPHYR_BASE
west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d $blinkCheckBuild "$learningRepo\stages\03_led_blink"
$LASTEXITCODE
```

| 項目 | 確認結果 |
|---|---|
| 使用版 | Zephyr 4.4.2、Python 3.12.10、west 1.5.0、CMake 3.31.8、SDK 1.0.1、GCC 14.3.0、DTC 1.6.1 |
| ターゲット | `frdm_mcxn947/mcxn947/cpu0` |
| 終了コード | 0（通常権限での再実行） |
| コンパイル・リンク | 157/157まで成功。成功ログにwarning／errorの検出なし |
| FLASH／RAM／SRAMX | 27,084 B／4,448 B／0 B |
| ELF／BINファイルサイズ | 729,316 B／27,084 B。ELFの大きさはFlash使用量とは異なる |
| LED・GPIO | `led0 = &red_led`、red_ledのgpiosはgpio0・ピン10・GPIO_ACTIVE_LOW。red_ledとgpio0はokay |
| 機能設定 | GPIO、GPIO_MCUX、PRINTK、CONSOLE、SERIAL、UART_CONSOLE、MULTITHREADINGがy。LEDサブシステムは無効 |
| 整数表示 | `CONFIG_CBPRINTF_FULL_INTEGRAL=y`。64ビット整数の時刻表示を組み込む設定。実際のUART表示は未確認 |
| システムtick | `CONFIG_SYS_CLOCK_TICKS_PER_SEC=10000`（1 tick = 0.1 ms）。`k_uptime_get()`の表示単位はmsであり、実機のLED周期は未測定 |
| 検証時main.cのSHA-256 | `3435F4B3FE694440C5CE61E2EC3F817C7E21AAAFF9AAAE27422044AA4D87B8E2` |

最初の制限付き実行では、DTCのMSYSランタイムがWindowsの名前付きオブジェクトを作成する際に
アクセス拒否（`0xC0000022`）で終了し、westの終了コードは1だった。
ソースを変更せず通常権限で同じビルドを再実行すると、DTC起動からリンクまで成功した。
この問題はCodex側の実行制限として記録し、本人の端末でのビルド失敗として扱わない。

- [成功した500 ms版のビルドログ](build-500ms.log)
- [制限付き実行での失敗ログ](build-500ms-restricted.log)
- [生成設定の抜粋](generated-settings.txt)

## 250 ms版

本人が定数を250へ変更した後、500 ms版と別の保存先でビルドした。
500 ms版で確認したDTCの実行制限を避け、通常権限で実行。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
$blinkCheckBuild = Join-Path $learningRepo '.tmp\build\03_led_blink_250ms_codex'
Set-Location $env:ZEPHYR_BASE
west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d $blinkCheckBuild "$learningRepo\stages\03_led_blink"
$LASTEXITCODE
```

| 項目 | 確認結果 |
|---|---|
| 使用環境・ターゲット | 500 ms版と同じ採用版、`frdm_mcxn947/mcxn947/cpu0` |
| 終了コード・コンパイル | 0、157/157まで成功。ログにwarning／errorの検出なし |
| FLASH／RAM／SRAMX | 27,080 B／4,448 B／0 B |
| ELF／BINファイルサイズ | 729,416 B／27,080 B |
| 生成設定 | `.config`と`zephyr.dts`の各SHA-256が500 ms版と一致。機能・LED・GPIO・tickの設定は同じ |
| 検証時main.cのSHA-256 | `F131B4144CFF60D7E443D10E1CBF0639EBBA57842B772FE4EC7E7CB055DA3713` |

[250 ms版のビルドログ](build-250ms.log)を保存した。実機の速度変更は未確認。
本人の実習結果は[段階3のREADME](../README.md#ユーザー本人の実習結果)へ別に記録する。
