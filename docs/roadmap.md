# 学習ロードマップ

段階0〜15をそれぞれ独立したイシューで管理します。2026-10-11時点で段階0のPC環境調査・選定・記録、段階1の本人によるビルド・書き込み・UART・GDB実習とPR確認、段階2の本人によるGPIOでのLED点灯・消灯、説明・記録・PR確認が完了しました。本人の承認後に[PR #17](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/17)と[PR #18](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/18)をマージし、#2・#3はcompletedとしてクローズ済みです。[段階1の実習結果](../stages/01_hello_world/README.md)、[段階2の実習結果](../stages/02_led_control/README.md)、[選定記録](../stages/00_versions/versions.md)を参照してください。段階3は本人の両版のビルド・書き込み・UART時刻差比較・LED点滅と速度変更・待機と周期の説明まで確認済みです。UARTとLEDの各状態の対応は500 ms版で確認し、250 ms版は速くて目視では断定できなかったと記録しています。最終版は250 ms。本人によるREADMEと[PR #20](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/20)の確認も完了し、マージとイシュー #4のクローズを承認済みです。[段階3の記録](../stages/03_led_blink/README.md)を参照してください。段階4は本人の環境確認、作業ブランチ作成、ソース・設定の表示確認とCPU0向けビルドまで完了しています。本人の接続先再確認と書き込みも成功表示・終了コード0で確認済みです。本人が3巡の低速→高速→消灯→高速再開→消灯と、途中で停止しないことを確認しました。UARTはseq 25まで進み、ERRORなし・set_retすべて0との報告です。提供UARTの時刻差も比較済みで、本人による役割分担・状態更新担当・待機・優先度・スタックの説明も確認済みです。READMEも本人が実際の学習・観察結果と一致すると確認し、PR作成を依頼しました。[PR #21](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/21)を作成し、本人によるPR内容の確認も完了しました。本人の別途の明示的な承認後、PR #21をマージし、イシュー #5をcompletedとしてクローズしました。段階4の学習は完了です。[段階4の記録](../stages/04_threads_queues/README.md)を参照してください。段階5は2026-10-12に本人の環境・Zephyrコミット・UART定義・作業ブランチ確認と、UART送信・LED・観察用待機・繰り返し、設定とCMakeの表示確認まで進みました。2026-10-13に本人のビルド抜粋と終了コード0を受領し、成功を確認しました。本人のCMakeCache・Kconfig・UART・LED・GPIOの生成定義確認も完了し、COM5とプローブHN2APUBRDCEPTも再確認済みです。本人の書き込みもFlash成功表示と終了コード0で確認済みです。本人のTera Term設定も確認済みです。RESET後の起動表示とcycle 1〜3のHello World受信を確認し、本人はLEDの点滅も報告しました。本人が送信ループ中と待機中のLED点滅継続、文字がゆっくり増える様子も確認しました。本人の出力経路・文字列終端・改行・待機・共有UARTの説明と、ログ比較の補足説明後の理解報告も受領しました。本人のREADME確認も2026-10-14に完了しました。本人の依頼によりCodexがコミット・プッシュし、[PR #22](https://github.com/grace2riku/zephyr_frdm_mcxn947/pull/22)を作成しました。本人のPR確認と別途の明示的な承認後、2026-10-14にPR #22をマージし、イシュー #6をcompletedとしてクローズしました。段階5の学習は完了です。[段階5の記録](../stages/05_uart_tx/README.md)を参照してください。段階6〜15は未着手です。

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
6. Flash／RAM使用量をバイト数とマイコン全体の容量に対する使用率（%）で記録する。2026-10-11の本人の指定により、次回の段階から適用する。画像・GUI段階ではバッファ構成と更新時間も記録する。
7. 学習に使用したAIツール、AIモデルの識別子・バージョン、推論設定、利用期間と確認根拠。[記録方法と後続段階の欄](ai-models.md)に従い、モデル変更時は行を追加する。

### Flash／RAM使用率の分母と確認範囲

[NXPのMCXN947仕様](https://www.nxp.com/part/MCXN947VKLT)を基に、内蔵Flashは2 MiB（2,097,152 B）、内蔵SRAMは512 KiB（524,288 B）を分母とする。使用率は「使用量（B）÷全体容量（B）×100」で計算し、小数点以下2桁を基本とする。

ビルドログのRAM領域は今回320 KiBなので、ログの1.36%（4,448 B÷327,680 B）と、マイコン全体に対する0.85%（4,448 B÷524,288 B）は分母が異なる。全体容量に対する割合を主に表示し、ビルドで割り当てられた領域の容量と使用率も区別して残す。SRAMXなど複数のRAM領域を使う場合は、mapファイルで領域の重複を確認し、重複しない使用量を合計する。

これらは対象アプリのビルド時に確定する使用量であり、実行時のスタック・ヒープの最大使用量や、別コアのプログラムを含めた実測値とは区別する。512 KiB全体をCPU0アプリが自由に使えることを意味しない。外付けメモリや、ECCなどによる利用可能容量の変更は別途明記する。

公開済みの公式資料の `latest` は変化します。実装時には採用したZephyr／Slintバージョンの資料とソースに照合します。
