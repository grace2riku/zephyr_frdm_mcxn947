# Codexによる段階2の事前検証

実施日：2026-10-10（日本時間）。実施者：Codex。**本人の実習結果ではない。**

点灯版（`LED_STATE=1`）をビルドし、生成された設定を確認した。
ボードへの書き込み、UART受信、実機の点灯・消灯は未実施。消灯版のビルドも未実施。

```powershell
$learningRepo = 'C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947'
. "$learningRepo\stages\01_hello_world\scripts\env.ps1"
$ledCheckBuild = Join-Path $zephyrWorkspace 'build/02_led_control_codex'
Set-Location $env:ZEPHYR_BASE
west build -p always -b frdm_mcxn947/mcxn947/cpu0 -d $ledCheckBuild "$learningRepo\stages\02_led_control"
```

| 項目 | 確認結果 |
|---|---|
| 使用版 | Zephyr v4.4.2（dccb09599635bdff17633fa7e9dab014b91dce90）、SDK 1.0.1、GNU GCC 14.3.0 |
| 終了コード | 0 |
| コンパイル／リンク | 成功。ログにwarning／errorの検出なし |
| Flash／RAM／SRAMX | 26,912 B／4,448 B／0 B |
| ELF／BINファイルサイズ | 727,724 B／26,912 B（ELFサイズはFlash使用量とは異なる） |
| alias | `led0 = &red_led` |
| red_led | `gpios = < &gpio0 0xa 0x1 >`、`status = "okay"` |
| gpio0 | `compatible = "nxp,kinetis-gpio"`、`nxp,kinetis-port = < &porta >`、`status = "okay"` |
| Kconfig | GPIO、GPIO_MCUX、PRINTK、CONSOLE、SERIAL、UART_CONSOLEがy、LEDサブシステムは無効 |
| 検証時main.cのSHA-256 | `52956AE5CAEC8D6E1977585BB600738CE1EAB01A851C231E8ED452BF816E459A` |

- [ビルドログ](build-on.log)
- [生成設定の抜粋](generated-settings.txt)（全体は上記ビルド先）

本人用の`build/02_led_control`はこの検証では使用していない。
本人からの報告は[段階2 README](../README.md#ユーザー本人の実習結果)へ別に記録する。
