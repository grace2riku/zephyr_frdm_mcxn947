# 段階7：本人から提供された記録

受領日：2026-10-31（JST）。操作した正確な日時は未取得。
Codexの事前検証とは区別する。

- `flash-verbose.log`：本人が添付した`west -v flash`のPowerShell出力。添付テキストを無変更でコピーし、SHA-256の一致を確認した。
- 入力：本人用`build/07_ntshell/zephyr/zephyr.bin`、0x10000000、31,376 B。
- 対象：MCXN947:FRDM-MCXN947、MCU-Link HN2APUBRDCEPT、LinkServer v1.4.85。
- 結果：`Finished writing Flash successfully.`、`Loaded 0x7A90 bytes`、終了コード0。
- `Sectors written: 0, unchanged: 4, total: 4`。今回の処理では4セクタが既存内容と一致し、追加のセクタ書き込みを行わず成功したとログに示されている。
- 補助のプローブブート処理のFailed to boot probes／UnicodeDecodeErrorは表示されたが、その後に上記成功表示と終了コード0がある。
- ローカルFW 3.133は更新候補の案内。更新実行の表示ではない。

この記録は書き込み処理の確認であり、UARTの起動表示やLEDコマンドの実機確認ではない。

## チャットで受領した実機確認報告

以下は2026-10-31に本人から受領した報告の要約であり、UART出力原文を再構成したものではない。

- 期待する起動表示とmcxn947>、赤色LEDの初期消灯。
- helpのUsage、led onで点灯、led offで消灯、各コマンド後のプロンプト復帰。
- 無入力とhelp入力途中でも点滅継続。led blink 250で速度上昇。led onで点滅停止・点灯維持、led offで消灯。
- unknown、led、led on extra、led blink 99／2001／-100／abcでERROR・Usageとプロンプト復帰。LEDは同じ速度で点滅継続し、最後のhelpとled offも正常。
- 空行・空白、Backspace編集、上矢印履歴、Ctrl+C取消し。取消したled onでは点灯せず、その後のhelpも正常。
- CR・LF・CR+LFの送信方式すべてでhelpが1回だけ実行されてプロンプトへ戻る。
- 最終の送信改行CR+LF・受信改行CRを明示確認。

LEDは本人の目視報告。厳密な周期・割り込み遅延・実行時最大メモリ使用量は測定していない。
UARTエラー／キュー満杯からの復帰と追加境界ケースの実機確認は未実施として、段階7のREADMEに残す。
