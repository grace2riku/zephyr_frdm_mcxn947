# 学習ロードマップ

段階0〜15をそれぞれ独立したイシューで管理します。2026-10-11時点で段階0のPC環境調査・選定・記録と、段階1の本人によるビルド・書き込み・UART・GDB実習・PR確認は完了しました。本人の承認後に[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)をマージし、#2をcompletedとしてクローズ済みです。[段階1の実習結果](../stages/01_hello_world/README.md)と[選定記録](../stages/00_versions/versions.md)を参照してください。段階2は[アプリと日本語手順](../stages/02_led_control/README.md)を準備し、Codexの点灯版ビルド・生成設定確認が済んでいます。2026-10-11に本人のセッション設定・入力ファイル・ボード定義の読み出しと、点灯版のビルド成功（終了コード0）を確認しました。生成設定、点灯版の書き込み成功（終了コード0）、UARTのAPI成功結果と赤色LEDの点灯を本人の報告で確認済み。本人による消灯版へのソース変更、ビルド・書き込み成功（各終了コード0）、UARTのAPI成功結果と赤色LEDの消灯を確認済み。本人のGPIO・極性の説明と記録の相違なしの確認を受領済み。PRの確認待ちで、#3はopenです。段階3〜15は未着手です。

| 段階 | 課題 | 追加するフォルダ | 前提段階 | イシュー |
|---|---|---|---|---|
| 0 | 使用バージョンと開発環境の方針を決定する | `stages/00_versions/` | なし | [#1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1) |
| 1 | 環境構築とHello Worldのビルド・実行・デバッグ | `stages/01_hello_world/` | [段階0](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1) | [#2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2) |
| 2 | GPIO APIでLEDを点灯・消灯する | `stages/02_led_control/` | [段階1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/2) | [#3](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) |
| 3 | k_msleep()でLEDを周期的に点滅させる | `stages/03_led_blink/` | [段階2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) | [#4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/4) |
| 4 | スレッドとメッセージキューでLEDを制御する | `stages/04_threads_queues/` | [段階3](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/4) | [#5](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) |
| 5 | UART APIでHello Worldを送信する | `stages/05_uart_tx/` | [段階4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) | [#6](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/6) |
| 6 | UARTのポーリング受信と割り込み受信でエコーを実装する | `stages/06_uart_echo/` | [段階5](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/6) | [#7](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/7) |
| 7 | NT-Shellを組み込みLED制御コマンドを追加する | `stages/07_ntshell/` | [段階6](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/7) | [#8](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/8) |
| 8 | NT-ShellにRAMのリード・ライト・ダンプを追加する | `stages/08_memory_ram/` | [段階7](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/8) | [#9](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/9) |
| 9 | NT-ShellでMCXN947の周辺レジスタを読み書きする | `stages/09_register_access/` | [段階8](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/9) | [#10](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/10) |
| 10 | LCD_PAR_S035に基本図形とカラーパターンを描画する | `stages/10_lcd_basic/` | [段階2](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/3) | [#11](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/11) |
| 11 | LCDに静止画像を表示し描画時間とRAM使用量を確認する | `stages/11_lcd_image/` | [段階10](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/11) | [#12](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12) |
| 12 | DVP 20-pin OV7670から画像を取得する | `stages/12_camera_capture/` | [段階4](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/5) | [#13](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/13) |
| 13 | OV7670のカメラ画像をLCD_PAR_S035に表示する | `stages/13_camera_display/` | [段階11](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12), [段階12](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/13) | [#14](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/14) |
| 14 | SlintのPC環境とZephyr向けビルド環境を構築する | `stages/14_slint_setup/` | [段階11](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/12) | [#15](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/15) |
| 15 | SlintでLCD_PAR_S035へ描画しUIを周期更新する | `stages/15_slint_display/` | [段階14](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/15) | [#16](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/16) |

## 進め方

- 基本は段階番号順に進めます。前提段階は技術的な依存関係です。
- LCD系は段階2、カメラ取得は段階4を終えれば着手できます。
- Slint系は段階11から進められ、カメラ系の完了を待つ必要はありません。
- 各段階を始める際に対応フォルダを追加し、動く成果物と実機確認の記録を残します。
- ユーザー本人がビルド・実機確認・デバッグなどの実習を行い、観察した結果を記録します。Codexは各操作の理由を日本語で説明し、結果を確認しながら支援します。
- Codexによる準備・事前検証と、ユーザー本人の実習結果は実施者を明記して区別します。Codexの検証だけでは学習の完了条件にチェックしません。
- ユーザーが質問し回答した内容は[QA表](qa.md)へ追記します。質問した段階、質問日時（日本時間）、カテゴリ、質問内容、回答を記録し、日時が不明または代替時刻の場合は明記します。
- ユーザー本人が完了条件を満たしたことを確認し、クローズを依頼または許可した場合のみ、イシューをクローズします。
- カメラ映像とSlint UIの統合、タッチ入力、デュアルコアは段階15以降の発展課題です。

## 各段階のREADMEに残す情報

1. 目的、前提段階、使用する機材と配線。
2. 使用バージョンと、前段階から変更した設定。
3. ビルド・書き込み・デバッグのコマンドと実行ディレクトリ。
4. 学ぶAPIとKconfig／Devicetreeの変更点。
5. 期待する動作、実際の確認結果、未確認事項、トラブル対応。
6. Flash／RAM使用量（画像・GUI段階ではバッファ構成と更新時間も記録）。

公開済みの公式資料の `latest` は変化します。実装時には採用したZephyr／Slintバージョンの資料とソースに照合します。
