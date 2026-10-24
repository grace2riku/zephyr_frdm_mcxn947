# Zephyr学習プロジェクト QA表

FRDM-MCXN947でZephyrを学ぶ過程で、ユーザーが質問し回答した内容を段階別に記録する。操作結果の報告は各段階のREADMEに残し、この表には疑問点とその回答を残す。

## 記録方針

- 日時は日本時間（Asia/Tokyo、JST、UTC+09:00）で記載する。
- 今回の過去分は、会話履歴の各質問に対応するターン開始時刻を「質問日時」欄に使用した。メッセージ送信時刻そのものは取得できていないため、両者を区別する。
- 質問・回答は意味を保って要約する。同じ質問に複数の論点がある場合は、同じ日時の別行に分ける。
- 表の回答だけでは理解しにくい内容には、欄外に図・具体例・補足説明を付け、回答欄からリンクする。会話で示した理解に役立つ図も記録に残す。
- 段階0より前の質問は「段階0前」と記載する。進捗への回答は質問当時の状態として読む。
- 今後も回答した質問を追記し、段階・質問日時・カテゴリ・質問内容・回答を残す。正確な日時を確認できない場合は不明または代替時刻の種類を明記し、記録した時刻を質問時刻として流用しない。

## QA一覧

| ID | 質問した段階 | 質問日時（JST、代替日時は注記） | 質問のカテゴリ | 質問内容 | 回答 |
|---|---|---|---|---|---|
| QA-001 | 段階0前：学習計画 | 2026-10-10 16:54:47 | 学習計画 | FRDM-MCXN947でZephyrを段階的に学ぶため、提示した課題より効率的な学習手順はありますか？ | 段階0〜15の計画を提案。Hello Worldの後にGPIO・LED点滅・スレッドとキュー・UART・NT-Shellを学び、メモリ操作をRAMと周辺レジスタに分ける。LCD・カメラ・Slintは単体で確認してから組み合わせる。各段階で動くコード、手順、期待動作、本人の確認結果を残す。詳細は[ロードマップ](roadmap.md)。 |
| QA-002 | 段階0前：進め方 | 2026-10-10 17:12:53 | チャット運用 | 段階ごとにチャットを分けたほうがよいですか？ | このプロジェクトでは1段階＝1チャットを基本とし、同じ段階の途中は同じチャットを使う。全体計画の相談と段階ごとの実習を分け、引き継ぎにはイシューとリポジトリ内のREADME・記録を使う。 |
| QA-003 | 段階0／#1 | 2026-10-10 17:28:19 | 進捗・完了条件 | 段階0は終了ですか？ | 質問時点でPC環境調査、使用バージョンの選定、日本語での理由・方針の記録は完了。ツール導入、Hello Worldのビルド・書き込み・デバッグは段階1の作業。この回答時点では#1はまだクローズしていなかった。 |
| QA-004 | 段階1／#2 | 2026-10-10 18:08:20 | 学習方針・イシュー管理 | 私がビルド・実機確認をしないうちに、学習用のイシュー#2をクローズするのはやめてもらえますか？ | Codexの検証だけでクローズした判断を訂正し、#2を再オープンした。Codexの準備・事前検証と本人の実習結果を区別する。本人が完了条件を満たしたことを確認し、クローズを依頼または許可した場合のみ閉じる。 |
| QA-005 | 段階1／#2 | 2026-10-10 20:16:59 | ビルド対象 | 「次は学習リポジトリに置いたHello Worldをビルド」とありますが、これまでは何をビルドしていたのですか？ | それまではZephyr本体の`zephyr/samples/hello_world`にある公式サンプルをビルドしていた。次は同じソースと設定をコピーした学習リポジトリの`stages/01_hello_world`を独立したアプリとしてビルドする。公式サンプルで環境の動作を確認し、その後、編集して学ぶアプリのビルドを確認する順番。 |
| QA-006 | 段階1／#2 | 2026-10-10 21:36:17 | GDB・トラブル対応 | GDBを起動しても`(gdb)`と表示されません。Enterキーを押したほうがよいでしょうか？ | `--Type <RET> for more, q to quit, c to continue without paging--`はページ送りの入力待ち。Enterで次のページ、`c`＋Enterで残りをまとめて表示する。`(gdb)`が出てから`set pagination off`を実行すると、その後のページ送り待ちを無効にできる。LinkServerのログが後から表示されても、GDBがページ送りを待っている場合がある。 |
| QA-007 | 段階1／#2 | 2026-10-10 22:19:09 | ビルドコマンド・PowerShell変数 | `west build`の`-d $helloBuild`は何を意味していますか？ | `-d`はビルド結果の保存先を指定するwestのオプション。`$helloBuild`はenv.ps1で設定したPowerShell変数で、今回の値は`C:\Users\grace\zephyrproject-mcxn947\build\01_hello_world`。ELF・BIN、途中のコンパイル結果、生成設定などがこのフォルダに保存される。 [欄外の保存先とビルドコマンドの図](#qa-007-supplement)を参照。 |
| QA-008 | 段階1／#2 | 2026-10-10 22:19:09 | ビルドコマンド・PowerShell変数 | `$learningRepo`は何を意味していますか？ | 学習リポジトリの場所を入れたPowerShell変数。今回の値は`C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947`。`"$learningRepo\stages\01_hello_world"`は実行前にアプリの絶対パスへ展開され、ビルド対象のソースと設定の場所としてwestへ渡される。変数名だけをPowerShellで入力すると値を確認できる。 [欄外の学習リポジトリのディレクトリ図](#qa-008-supplement)を参照。 |
| QA-009 | 段階1／#2 | 2026-10-10 22:29:36 | プロジェクト構成・標準方式 | ソースコードとビルド結果の格納場所を分けているのですか？ それがZephyrの標準的な構成ですか？ | 生成物を専用のビルドディレクトリに分けることはZephyrの基本方式。ただし今回のように別の大きなフォルダへ置くことは必須ではなく、アプリ内の`app/build`へ出力する構成も使える。ワークスペースの外に学習アプリを置く今回の方式は、公式に対応したfreestanding application。学習コード・記録のGit管理と、依存コード・生成物の管理を分ける目的で採用した。[公式資料](https://docs.zephyrproject.org/latest/develop/application/index.html)。 [欄外のソースとビルド結果の関係図](#qa-009-supplement)を参照。 |
| QA-010 | 段階1／#2 | 2026-10-10 22:38:35 | 編集範囲・環境管理 | 学習リポジトリにアプリのソースを置き、Zephyrワークスペースではビルド結果だけを追加し、他のファイルを編集しないという理解でよいですか？ | 学習アプリのソースと設定を学習リポジトリに置き、普段はZephyr本体・依存ライブラリを直接編集せずに使う方針。ただしワークスペースではビルド結果以外にも設定・キャッシュ・ログが生成され、環境構築や更新でツール・依存パッケージも追加・更新される。 [欄外の編集・生成・環境管理の区分図](#qa-010-supplement)を参照。 |
| QA-011 | 段階1／#2 | 2026-10-10 22:38:35 | ワークスペース・ボード対応 | Zephyrワークスペースには具体的に何が入っていますか？ Zephyrのソース、ツール、FRDM-MCXN947のコードなどでしょうか？ | `zephyr/`にカーネル・ドライバ・公式サンプル・ボード対応、`modules/`に外部ライブラリやNXPの制御コード、`bootloader/`にMCUboot、`tools/`に関連ツール、`.venv/`にPython環境、`.west/`にwest管理情報、`build/`に生成物、`downloads/`に取得した配布物がある。ボード対応は`zephyr/boards/nxp/frdm_mcxn947`、NXPのMCXN947対応は`modules/hal/nxp/mcux/mcux-sdk-ng/devices/MCX/MCXN/MCXN947`。SDKは別フォルダ。 [欄外のワークスペースのディレクトリ図](#qa-011-supplement)を参照。 |
| QA-012 | 段階1／#2 | 2026-10-10 22:41:39 | 開発環境の全体像 | Zephyr SDK、ワークスペース、学習リポジトリ、開発環境の全体像を教えてください。 | 学習リポジトリは編集するアプリと記録、ワークスペースはZephyr本体・依存コードと今回配置した環境・生成物、SDKはGCC・リンカ・GDBなどの道具。VS Codeで編集し、PowerShellからwestを操作する。west buildはCMake・NinjaとSDKでELF・BINを生成。west flash/debugはLinkServerとMCU-Linkで実機を操作し、Tera TermはCOM5からUARTを受信する。env.ps1がPATH、Zephyr・SDKの場所、各パス変数を設定してこれらを結び付ける。[欄外の構成図と読み方](#qa-012-supplement)を参照。 |
| QA-013 | 段階1／#2 | 2026-10-10 23:05:41 | Git・PR・イシュー管理 | 段階1の確認はOKだと思います。プルリクエストを作成し、イシューをクローズするのが一般的なプロセスですか？ | はい。本人の実習確認後、変更をコミット・pushし、PRで差分と確認結果をレビューしてからマージし、イシューをクローズする流れが適している。PRは変更を確認・取り込むため、イシューは学習課題の完了を追跡するために使う。PR本文に`Closes #2`と書き、既定ブランチへマージすると#2を自動クローズできる。PRを作成しただけでは閉じない。[GitHub公式資料](https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/linking-a-pull-request-to-an-issue)。質問時点で本人の確認OKを受け、PR作成・マージとクローズは未実施。 |
| QA-014 | 段階2／#3 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | CMake・相対パス | `target_sources(app PRIVATE src/main.c)`は学習リポジトリの`stages/02_led_control/src/main.c`を指しますか？ なぜその場所になるのですか？ | はい。`target_sources`の相対ソースパスは、CMakeが現在処理しているソースディレクトリ（`CMAKE_CURRENT_SOURCE_DIR`）を基準にする。今回の行は段階2のCMakeLists.txtにあるため、`stages/02_led_control`と`src/main.c`を組み合わせた場所になる。PowerShellの現在位置やビルド出力先を基準にしない。採用CMake 3.31.8の同梱公式Helpと、Codexの事前ビルドで生成されたbuild.ninjaのコンパイル対象で確認。[欄外のパスの具体例](#qa-014-supplement)。 |
| QA-015 | 段階2／#3 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | C言語・コンソール出力 | printfとprintkは何が違うの？ | `printf()`はC標準ライブラリの関数で、`<stdio.h>`を読み込み標準出力stdoutへ出力する。`printk()`はZephyrの確認・デバッグ用コンソール出力関数で、`<zephyr/sys/printk.h>`を読み込む。printfはint（成功時は出力文字数、エラー時は負値）、printkはvoid。今回の設定では両方とも同じUARTコンソールへつながる。GPIO APIの状態や戻り値を表示する目的でprintkを採用した。採用版では書式処理やサイズはCライブラリと設定に依存し、printkのほうが必ず小さいとは断定しない。[欄外の比較と実装確認](#qa-015-supplement)。 |
| QA-016 | 段階2／#3 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | 学習手順・Devicetree | 次は何を学びますか？ | 次はDevicetreeで使用LED・GPIO・極性を調べ、CコードのDT_ALIASとGPIO_DT_SPEC_GETに対応づける。共通定義のled0→red_led、gpio0のピン10・GPIO_ACTIVE_LOW、CPU0定義でのred_led・gpio0のokayを本人が表示して確認する。その後、GPIO準備状態・出力設定・論理値設定と戻り値を学び、本人のビルド・生成設定確認・書き込み・点灯／消灯へ進む。まずはボード定義の読み出しだけを案内し、本人の結果を待つ。[段階2の操作2](../stages/02_led_control/README.md#操作2：3つの入力ファイルを読む)。 |
| QA-017 | 段階2／#3 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | Devicetree・有効化 | 共通定義のdisabledをCPU0側でokayにしていますが、okayとは何を意味しますか？ | `status = "okay"`はDevicetreeのノードを使用対象として有効にする設定。`disabled`は明示的な無効化。CPU0側の設定で同じred_ledノードのstatusを上書きする。okayはLED点灯の命令や正常動作確認の結果ではない。GPIOドライバの使用にはKconfigなどの条件も必要で、実行時の初期化状態はgpio_is_ready_dt、実際の出力はgpio_pin_configure_dt／gpio_pin_set_dtで確認・設定する。[欄外の設定と実行時状態の区別](#qa-017-supplement)。 |
| QA-018 | 段階2／#3（完了後の補足） | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | Devicetree・ビルド生成物 | `zephyr.dts`は何から参照され、どこに作成され、どんな役割をもっていますか？ | `west build`のCMake設定処理で、選択したボードの.dtsと、それが読み込む.dtsi、指定があればoverlayを統合して生成する。今回は`$ledBuild/zephyr/zephyr.dts`。最終的なハードウェア設定を人が確認するための出力で、DTCも追加の警告・エラー検査に読む。Cコードが直接読むファイルではなく、同じ解析結果から生成したdevicetree_generated.hをZephyrのヘッダ経由で参照する。GPIO APIへ渡すポート・ピン・極性はコンパイル時に取り込まれる。変更は入力側へ行い、再ビルドする。[入力・生成・参照のつながり](#qa-018-supplement)。 |
| QA-019 | 段階2／#3（完了後の補足） | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | Devicetree・overlayの判断 | overlayとは何ですか？ 使う／使わない判断と、今回不要と判断した理由は？ | overlayはボードのDevicetreeへアプリ用の設定を追加・上書きする入力ファイル。使いたいハードウェア構成と、対象ボードの最終Devicetreeのalias・有効化状態・GPIO／ピン・極性などを比べ、差分があればoverlayで調整する。既定設定で足りれば追加不要。今回はled0が赤色LEDを指し、CPU0用定義でred_ledとgpio0がokay、GPIO0のピン10・Active Lowも目的に一致し、GPIOドライバが出力・ピンmuxを設定するので追加のDevicetree設定は不要。Kconfigの機能選択はprj.conf、点灯・消灯の論理値はmain.cで扱う。[判断例と今回の根拠](#qa-019-supplement)。 |
| QA-020 | 段階4／#5 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | スレッド・受信待機 | ソースの先頭40行を確認しました。次のLEDスレッドの受信・待機処理を説明してください。 | LEDスレッドは停止中ならk_msgq_getでK_FOREVER、点滅中ならK_MSEC(interval_ms)を指定する。キューに指示があれば即時受信し、空なら指示か時間満了まで待機してCPUを譲る。戻り値0なら構造体へ指示がコピーされ、後続のswitchで処理する。点滅中の-EAGAINは時間満了なのでLEDを反転し、continueで次の待機へ進む。停止中も指示が届けば再開できる。待機中はGPIO出力を保持し、running・logical・interval_msはLEDスレッドだけが更新する。[段階4の受信待ちと点滅](../stages/04_threads_queues/README.md#受信待ちと点滅)を参照。 |
| QA-021 | 段階4／#5 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | 指示反映・状態更新 | 71〜115行を確認しました。次のswitchによるSTART・STOP・間隔変更の処理を説明してください。 | 受信成功後にcommand.typeでcaseを選ぶ。STARTは停止中だけ点灯し、成功後にlogical=1・running=trueへ更新。STOPは消灯しlogical=0・running=falseにするがスレッドは終了しない。両方とも間隔は保持。SET_INTERVALは100〜2000 msの値だけ受け付け、interval_msだけを更新し、点滅中なら次の受信待機から新しい間隔を使う。GPIOの現在値は変えず、停止中にも設定できる。breakはswitchを抜けてwhileを継続し、GPIO異常時のreturnはスレッドの関数を終了する。重複STARTや不正指示でも次の待機は開始し直す。[段階4の指示反映](../stages/04_threads_queues/README.md#switchによる指示の反映)を参照。 |
| QA-022 | 段階4／#5 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | 指示スレッド・送信待機 | 117〜154行を確認しました。指示スレッドがSTART・STOP・間隔変更の指示を送る処理を説明してください。 | 指示スレッドは固定の8項目のscriptをforで順番に選び、type・interval_ms・連続番号を入れた構造体をk_msgq_putで送る。K_MSEC(1000)はキュー満杯時の最大待機で、必ず1秒待つ意味ではない。送信成功はLED操作完了の応答ではない。成功後、hold_msが正なら指示スレッドだけがk_msleepで待ち、LED側は点滅を継続できる。interval_msはLEDのON/OFF切り替え間隔、hold_msは次の指示までの待機で、後者はメッセージに含めない。whileで全操作列を繰り返し、送信失敗は表示して指示スレッドを終了する。[段階4の送信と待機](../stages/04_threads_queues/README.md#指示スレッドの送信と待機)を参照。 |
| QA-023 | 段階4／#5 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | スタック・優先度・スレッド作成 | 156〜199行を確認しました。スタック・優先度と、mainが2つのスレッドを作成・開始する処理を説明してください。 | K_THREAD_STACK_DEFINEで各スレッド専用のスタックを用意し、struct k_threadで別に制御ブロックを用意する。2048 Bは要求サイズで実行時最大使用量ではない。優先度は小さい数値が高く、今回main=0、LED=5、指示=6。高優先度でも待機中はCPUを使わない。mainはk_thread_createに制御ブロック・スタック・専用サイズマクロ・入口関数・未使用引数NULL・優先度・オプション0・開始遅延K_FOREVERを渡し、両方を作成後にk_thread_startで実行可能にする。mainのreturnはmainスレッドだけを終了し、追加スレッドはCPU0で処理を継続する。[段階4の作成と開始](../stages/04_threads_queues/README.md#スタックと制御ブロックの準備mainでの開始)を参照。 |
| QA-024 | 段階4／#5 | 2026-10-11（受領日を代替記載。質問の正確な日時は不明） | 繰り返し確認・UART時刻差 | 最初の1巡を確認しました。次の繰り返し動作と時刻差の比較へ進んでください。 | 受信を記録してからRESETし、seq 1〜24の3巡と次のseq 25まで約50秒観察する。低速→高速→消灯→高速再開→消灯が続き、ERRORや負のset_retがないことを確認する。時刻差は指示を挟まないreason=TIMEOUT・logical=1の行同士で比較する。今回の提供UARTをCodexが集計すると500 ms設定は1012 ms、250 ms設定は512〜513 ms。名目周期は待機2回分だが、表示などの処理時間も含む。LEDの光の周期の測定ではない。[段階4の比較結果](../stages/04_threads_queues/README.md#提供uartの時刻差比較)。 |
| QA-025 | 段階4／#5 | 2026-10-11（受領日を代替記載。回答の正確な日時は不明） | 理解確認・状態更新担当 | mainが2スレッドを生成・起動し、指示側が開始・停止・周期・点滅時間を送り、LED側が受信して制御する。状態はLED側が更新し、担当を1つにすると排他制御が不要という理解でよいですか？ | 役割分担と状態更新担当、当該状態のスレッド間排他制御が不要になる点は正しい。送るのは指示の種類・ON/OFF切り替え間隔interval_ms・指示番号。hold_msは指示スレッドが次の指示までk_msleepで待つ時間で、LED側へ送信しない。例えばSTART後に指示側が4000 ms待ってからSTOPを送ることで、約4秒間点滅させる。interval_ms=500なら名目ON→ON周期は1000 ms。キューの送受信同期はk_msgqが担う。[段階4の送信と待機](../stages/04_threads_queues/README.md#指示スレッドの送信と待機)。 |
| QA-026 | 段階4／#5 | 2026-10-11（受領日を代替記載。回答の正確な日時は不明） | 理解確認・送受信の待機 | 停止中はK_FOREVER、点滅中はK_MSEC(interval_ms)。k_msleepで待つのは指示側だけ。送信はキュー満杯時に最大1秒待つという理解の確認。 | 3点とも正しい。停止中の受信待ちは指示が届けば解除される。点滅中は指示受信または時間満了まで待ち、時間満了ならLEDを反転する。これらの待機中はCPUを他の実行可能な処理へ譲る。指示側のk_msleepはLED側の待機や点滅を止めない。送信はキュー満杯時に空きができるまで最大1秒待ち、空きができれば早く復帰する。[段階4の受信待ち](../stages/04_threads_queues/README.md#受信待ちと点滅)。 |
| QA-027 | 段階4／#5 | 2026-10-11（受領日を代替記載。回答の正確な日時は不明） | 理解確認・優先度とスタック | 小さい数値が高優先度でLED側が優先され、受信待ちなら指示側が動ける。スタックはローカル変数や関数呼び出し情報の保存領域で、2048 Bは最大使用量の測定値ではないという理解の確認。 | 3点とも正しい。優先度は実行可能なスレッドを選ぶ際に使い、待ち状態のLED側は選択対象にならない。2048 Bはスタックの要求サイズであり、実行時の最大使用量は別途測定が必要。本段階では最大使用量を測定していない。[段階4のスレッド構成](../stages/04_threads_queues/README.md#スレッドの役割優先度スタック)。 |
| QA-028 | 段階5／#6 | 2026-10-11（依頼受領日を代替記載。正確な日時は不明） | コンソール・UART送信経路 | 段階4までのprintkによる表示と、UARTデバイスを指定してuart_poll_outで1文字ずつ送る処理の違いを説明してください。 | 今回の設定ではprintkは書式処理後、UARTコンソールの文字出力フックを経てuart_poll_outへ渡す。直接送信はアプリがUARTデバイスと1文字を指定する。同じchosenのUARTを使えば物理的な出口は共有される。コンソールはLFの前にCRを追加し、直接送信では送信する文字・改行をアプリが選ぶ。採用MCUX LPUARTドライバは送信可能状態をwhileで確認するため、k_msleepのように自発的にCPUを譲る待機とは区別する。ローカルのコンソール・ドライバ・APIに照合した説明で、段階5の実機確認は未実施。[段階5の経路と確認範囲](../stages/05_uart_tx/README.md#コンソール表示とuart-apiの経路)。 |
| QA-029 | 段階5／#6 | 2026-10-13（受領日を代替記載。質問の正確な日時は不明） | PowerShell・学習再開 | 一度ターミナルを終了した後は何のコマンドを実行すればよいですか？ | 新しいPowerShellでlearningRepoを設定し、段階1のenv.ps1をドットソースで読み込む。env.ps1でzephyrWorkspaceなどが設定されるため、その後uartTxBuildをbuild/05_uart_txへ設定し、ZEPHYR_BASEへ移動する。uartTxBuildはenv.ps1にないため別に設定する。端末終了で変数・セッション環境設定は失われるが、ソース・ブランチ・ビルド生成物は残る。質問時点は本人ビルド成功済み、生成設定の本人確認前なので、再ビルドせずCMakeCacheと.configの確認から再開できる。[段階5の再開手順](../stages/05_uart_tx/README.md#ターミナルを閉じた後の再開)。 |
| QA-030 | 段階5／#6 | 2026-10-13（受領日を代替記載。質問の正確な日時は不明） | 理解確認・コンソールとUART API | [console]行はprintk、Hello Worldはuart_send_string内のuart_poll_out。printkは書式処理とコンソールを経由し、直接送信はUARTデバイスを指定して1文字ずつ送る。出口は同じUARTという理解で合っていますか？ | はい。今回の設定はUARTコンソールと直接送信が同じchosenのUARTを使う。printkも最終的にはコンソールのconsole_outからuart_poll_outへ渡す。直接送信は補助関数のループで各文字をuart_poll_outへ渡す。UARTへ届く前の書式処理・コンソール処理の経路が異なる。本人の出力経路の説明を確認済み。コンソールのLFへのCR追加と直接送信のCRLF指定は、終端・改行の理解確認で続けて確認する。[段階5の経路](../stages/05_uart_tx/README.md#コンソール表示とuart-apiの経路)。 |
| QA-031 | 段階5／#6 | 2026-10-13（受領日を代替記載。質問の正確な日時は不明） | 理解確認・文字列終端と送信範囲 | 末尾のNULはwhile条件が偽になって送信前に抜けるので送らない。Hello World!の12バイトとCRLFの2バイトで合計14バイトを送るという理解で合っていますか？ | はい。現在のASCII文字列では本文12バイトとCR・LF各1バイトの合計14バイトを送る。終端NULは値0で、while条件によりUART APIへ渡さない。C文字列の保存上はNULを含む15バイト。本人が条件とバイト数を説明できたことを確認済み。[段階5の文字列終端](../stages/05_uart_tx/README.md#文字列終端とポーリング送信)。 |
| QA-032 | 段階5／#6 | 2026-10-13（回答受領日を代替記載。正確な日時は不明） | 理解確認・改行とコンソール変換 | printkはLF出力後にCRを付ける。LFで改行、CRで行頭へ移動するという説明の確認。 | CRで行頭へ移動し、LFで次の行へ進む点は正しい。送信順序は修正し、採用UARTコンソールはLFを送る前にCRを追加するため、UARTへ出る順序はCR→LF。直接送信は文字列に指定したCR→LFをそのまま送る。LF自体には行頭へ戻す指定は含まれない。採用版uart_console.cの99〜103行をCodexが照合して補足。その後、本人が該当コードを提示し、CRの後にLFを送る順序を確認した。[段階5の改行確認](../stages/05_uart_tx/README.md#コンソール表示とuart-apiの経路)。 |
| QA-033 | 段階5／#6 | 2026-10-13（回答受領日を代替記載。正確な日時は不明） | 理解確認・共有UARTの出力混在 | LEDスレッドが点滅のたびにprintkするなら、Hello Worldの1行はまとまらず混じって表示されるという説明の確認。 | 混在する可能性があるという理解は正しい。必ず混じるとは限らず、1行がまとまる保証がないという意味。直接送信は1文字ごとの呼び出しで、同じUARTの指定だけでは文字列全体を保護しない。現在の正常動作ではmainが順番に出力し、LED側は出力しない。GPIOエラー時にはLED側のprintkが混在し得る。混在を発生させる実機実験は未実施。[段階5の共有UART](../stages/05_uart_tx/README.md#コンソール表示とuart-apiの経路)。 |
| QA-034 | 段階5／#6 | 2026-10-13（回答受領日を代替記載。正確な日時は不明） | 理解確認・各文字後の待機とポーリング | 100 ms待機はuart_send_string内でuart_poll_outから戻った後にk_msleep(100)を実行する。ドライバ内のwhileはmainを待ち状態にせずCPUでフラグを繰り返し確認し、自発的にCPUを譲る待機ではないという理解で合っていますか？ | はい。100 ms待機では呼び出したmainスレッドが待ち状態になり、LEDなど他の実行可能なスレッドへCPUを譲る。採用ドライバ内のwhileはCPUを使ってフラグを繰り返し読み、自発的に待ち状態にはならない。高優先度スレッドによるプリエンプションは別の仕組み。本人が待機の位置とCPUの扱いの違いを説明できたことを確認済み。[段階5の待機処理](../stages/05_uart_tx/README.md#コンソール表示とuart-apiの経路)。 |
| QA-035 | 段階5／#6 | 2026-10-13（質問受領日を代替記載。正確な日時は不明） | ログの遅延処理とUART送信 | 「ログの遅延処理とUARTのポーリング送信は両立するか」という質問の意味が分からないので、具体的に教えてください。 | ログを有効にして遅延処理を選んだ例では、LOG_INFでメッセージをメモリのバッファへ登録し、後でログ処理の担当がUARTバックエンドへ渡す。バックエンドがポーリングの経路を使う場合は各バイトをuart_poll_outへ渡し、ドライバが送信可能フラグを確認する。「後で処理する時期」と「実際にUARTへ送る方法」が別なので両方を使える。今回のCONFIG_LOG=nで実行した処理ではなく、ソースによる説明用の例。本人は補足説明後に理解したと報告し、記録確認へ進んだ。[段階5のログ比較](../stages/05_uart_tx/README.md#ログ出力との比較)。 |
| QA-036 | 段階5／#6 | 2026-10-13（質問受領日を代替記載。正確な日時は不明） | ログ説明の表現 | ログの遅延の意味は説明で分かった。「後で送ることと、送る際にポーリングすることは矛盾しません」は何と何を指しているのですか？ | 「ログを後で処理する」と「実際のUART送信時にポーリングする」の両方を同じ構成で使えるという意味。保存したログを、後でUARTバックエンドがuart_poll_outで送れる、という説明で十分だった。本人はログの遅延の意味が分かったと報告済み。ログ有効化の実機結果ではなく、採用ソースに基づく説明。[段階5のログ比較](../stages/05_uart_tx/README.md#ログ出力との比較)。 |

## 欄外の補足

### 図で使用するパス

以下は今回の実習環境のパス。図では読みやすくするため「学習リポジトリ」「ワークスペース」などの名称で示す。

| 図の名称・変数 | 今回の絶対パス | 役割 |
|---|---|---|
| 学習リポジトリ：`$learningRepo` | `C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947` | 自分のアプリ、学習手順、QAを保存する |
| 学習アプリ | `C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947\stages\01_hello_world` | Hello Worldのビルド入力 |
| ワークスペース：`$zephyrWorkspace` | `C:\Users\grace\zephyrproject-mcxn947` | Zephyr本体、依存コード、今回配置したツールとビルド結果 |
| 通常ビルド：`$helloBuild` | `C:\Users\grace\zephyrproject-mcxn947\build\01_hello_world` | 通常のビルド結果を保存する |
| デバッグビルド：`$helloDebugBuild` | `C:\Users\grace\zephyrproject-mcxn947\build\01_hello_world_debug` | デバッグ用の設定で作ったビルド結果を保存する |
| Zephyr SDK | `C:\Users\grace\zephyr-sdk-1.0.1` | コンパイラ、リンカ、GDBなど |

<a id="qa-007-supplement"></a>

### QA-007：`-d $helloBuild`とビルド結果の保存先

実習で使ったコマンドを、入力と出力に分けて見る。

```powershell
west build -p always -b frdm_mcxn947/mcxn947/cpu0 `
    -d $helloBuild "$learningRepo\stages\01_hello_world"
```

```mermaid
flowchart LR
    Source["最後の引数<br>学習リポジトリのstages/01_hello_world<br>ソースと設定の入力"] --> Build["west build"]
    Board["-b frdm_mcxn947/mcxn947/cpu0<br>対象ボード・CPU0の指定"] --> Build
    Destination["-d $helloBuild<br>ワークスペースのbuild/01_hello_world<br>出力先の指定"] -.-> Build
    Build --> Result["$helloBuildに生成"]
    Result --> Elf["zephyr/zephyr.elf<br>実行コード・デバッグ情報"]
    Result --> Bin["zephyr/zephyr.bin<br>書き込み用バイナリ"]
    Result --> Intermediate["CMakeCache.txt・build.ninjaなど<br>ビルド設定・途中の生成物"]
```

`-d`は保存先を選ぶ指定で、ソースの場所は最後の引数で指定する。`-p always`は、指定したビルドディレクトリの既存の生成物を消してから作り直す指定。上のコマンドは説明用の再掲なので、図を読むために再実行する必要はない。

確認するときはPowerShellで`$helloBuild`の値を表示し、そのフォルダの中に`zephyr\zephyr.elf`や`zephyr\zephyr.bin`ができているかを見る。

<a id="qa-008-supplement"></a>

### QA-008：`$learningRepo`から学習アプリまでの位置関係

`$learningRepo`は学習リポジトリ全体の場所。`stages\01_hello_world`を付け足すと、今回ビルドするアプリの場所になる。線はフォルダの親子関係を示す。

```mermaid
flowchart TD
    Repo["$learningRepo<br>zephyr_frdm_mcxn947"] --> Docs["docs<br>ロードマップ・QA表"]
    Repo --> Stages["stages<br>段階ごとのアプリと記録"]
    Stages --> App["01_hello_world<br>今回のビルド対象"]
    App --> Source["src/main.c<br>実行するCコード"]
    App --> Config["prj.conf<br>Zephyrの機能設定"]
    App --> CMake["CMakeLists.txt<br>アプリのビルド定義"]
    App --> Debug["debug.conf<br>デバッグビルド時の追加設定"]
    App --> Scripts["scripts/env.ps1<br>PowerShellの変数と環境を設定"]
    App --> Notes["README.md・evidence<br>手順・確認結果"]
```

PowerShellの二重引用符の中では変数が展開される。`"$learningRepo\stages\01_hello_world"`は、上のパス表にある「学習アプリ」の絶対パスになる。`$learningRepo`そのものを指定すると、アプリより上のリポジトリ全体を指してしまう。

`$learningRepo`と`"$learningRepo\stages\01_hello_world"`をそれぞれPowerShellで表示すると、どこまでを変数に入れ、どこを付け足したか確認できる。`debug.conf`はファイルを置くだけでは追加されず、デバッグ用ビルドで指定して使う。

<a id="qa-009-supplement"></a>

### QA-009：ソースとビルド結果を分ける今回の構成

この図の線はフォルダの親子関係ではなく、ビルドに使う入力と生成物の流れを示す。

```mermaid
flowchart LR
    subgraph Learning["学習リポジトリ"]
        App["stages/01_hello_world<br>main.c・prj.conf・CMakeLists.txt"]
    end
    subgraph Work["Zephyrワークスペース"]
        Zephyr["zephyr<br>OS・ドライバ・ボード設定"]
        Modules["modules<br>依存ライブラリ・NXPのコード"]
        Output["build/01_hello_world<br>ビルド結果"]
    end
    Sdk["ワークスペース外のZephyr SDK<br>コンパイラ・リンカ"]
    App -->|"最後の引数で指定"| Build["west build"]
    Zephyr --> Build
    Modules --> Build
    Sdk --> Build
    Build -->|"-d $helloBuildで保存先を指定"| Output
```

アプリのソースとビルド結果は別のフォルダにあり、ビルドでは自分のアプリとZephyr側のコードを組み合わせる。学習リポジトリを編集しても、生成物の保存先は`-d`の指定に従う。

ソースと生成物を分けるために、今回の2つのルートフォルダへ分割することまでは必須ではない。アプリ内に専用の`build`フォルダを作る配置も使える。今回は、自分が管理する学習コード・記録と、Zephyrの依存コード・生成物を分けるためにこの配置を選んだ。

<a id="qa-010-supplement"></a>

### QA-010：普段編集するものと、ビルドや環境管理で変わるもの

矢印のラベルは「誰が、どのように扱うか」を示す。

```mermaid
flowchart TD
    Learner["学習者"] -->|"編集・記録"| App["学習リポジトリ<br>src/main.c・prj.conf・README.md・QA表"]
    App -->|"入力として読む"| Build["west build"]
    Vendor["ワークスペースのzephyr・modulesなど<br>普段のアプリ学習では直接編集せず使う"] -->|"入力として読む"| Build
    Build -->|"生成・更新"| Generated["ワークスペースのbuild<br>ELF・BIN・生成設定・途中のファイル"]
    Build -->|"必要に応じて更新"| Cache["ワークスペース内のキャッシュ<br>例：zephyr/.cache"]
    Setup["環境構築・依存コードの更新"] -->|"導入・更新"| Managed["ワークスペースの.venv・tools・.west<br>zephyr・modules・bootloaderなど"]
```

普段の実習では、自分のアプリと学習記録を編集する。一方、ワークスペースはビルド結果を保存するだけの場所ではなく、ツールや依存コードも持つ。ビルドでキャッシュが更新され、環境構築や更新で依存コード・Pythonパッケージなどが変わることもある。

したがって「Zephyr本体や依存ライブラリを普段は直接編集しない」が今回の方針。「ワークスペースではビルド結果以外は一切変わらない」という意味ではない。

<a id="qa-011-supplement"></a>

### QA-011：ワークスペースの中身とFRDM-MCXN947のコード

以下は実際のワークスペースの主要フォルダ。線はフォルダの親子関係を示し、深い階層は途中を省略して1つの枠にまとめている。`tools`・`downloads`・`.venv`などの配置は今回の環境構築で選んだもの。

```mermaid
flowchart TD
    Root["$zephyrWorkspace<br>zephyrproject-mcxn947"] --> Zephyr["zephyr<br>Zephyr本体"]
    Zephyr --> Core["kernel・drivers・include・samples<br>OS・ドライバ・ヘッダ・公式サンプル"]
    Zephyr --> Board["boards/nxp/frdm_mcxn947<br>基板のピン・周辺機器・CPU0などの設定"]
    Root --> Modules["modules<br>外部の依存コード"]
    Modules --> Nxp["hal/nxp/mcux/mcux-sdk-ng<br>NXPのハードウェア制御コード"]
    Nxp --> Device["devices/MCX/MCXN/MCXN947<br>MCXN947向けのデバイス定義など"]
    Root --> Boot["bootloader<br>MCUbootなど"]
    Root --> Tools["tools<br>今回配置したCMake・Ninja・dtc・gperf"]
    Root --> Venv[".venv<br>Python・west・必要なPythonパッケージ"]
    Root --> West[".west<br>westのワークスペース管理情報"]
    Root --> Downloads["downloads<br>今回取得した配布物"]
    Root --> Builds["build<br>ビルド結果"]
    Builds --> Normal["01_hello_world<br>通常のビルド結果"]
    Builds --> Debug["01_hello_world_debug<br>デバッグ用のビルド結果"]
```

FRDM-MCXN947のボード設定と、MCXN947というチップを制御するNXPのコードは別の場所にある。ボード設定は「この基板でどのピン・周辺機器を使うか」、NXP側のコードは「チップのハードウェアをどう制御するか」をビルドに提供する。

ワークスペースに存在するコードすべてをHello Worldに組み込むわけではなく、対象ボードと機能設定に必要なものをビルドで選ぶ。たとえば`bootloader`にMCUbootのコードがあっても、今回のHello WorldでMCUbootを使っているという意味ではない。

Zephyr SDKはこのツリーの外に置いている。NXPの`mcux-sdk-ng`はチップ制御のコード、Zephyr SDKはGCC・リンカ・GDBなどの道具という違いがある。


<a id="qa-012-supplement"></a>

### QA-012：開発環境の構成図と読み方

ソースコードを編集してから、ビルド・書き込み・デバッグ・UART出力の確認までの関係を示す。以下は今回の実習環境の構成で、ビルド結果はワークスペースの`build`フォルダに保存する。

```mermaid
flowchart TD
    Editor["VS Codeで編集"] --> App["学習リポジトリ<br>main.c・prj.confなど"]

    App --> Build["west build<br>CMake・Ninjaでビルド"]
    Workspace["ワークスペース<br>Zephyr本体・ボード対応・依存コード"] --> Build
    Compiler["Zephyr SDK<br>GCC・リンカ"] --> Build

    Build --> Output["ワークスペースのbuildフォルダ<br>zephyr.elf・zephyr.bin"]
    Output -->|"west flashで書き込み"| Link["LinkServer"]
    Output -->|"ELFを読み込む"| Debugger["SDKのGDB<br>west debugで起動"]
    Debugger <-->|"停止・実行・メモリ確認"| Link

    Link <-->|"J17のUSB接続"| Probe["ボード上のMCU-Link"]
    Probe <-->|"SWD"| CPU["MCXN947のCPU0"]

    CPU -->|"UART出力をMCU-Link経由で受信"| Terminal["Tera Term<br>COM5"]
```

図は次の順に読む。

1. **ビルド**：自分のアプリとZephyrのコード・ボード設定を入力にし、SDKのコンパイラとリンカで実行ファイルを作る。CMakeがビルド手順を生成し、Ninjaがその手順を実行する。
2. **書き込みとデバッグ**：LinkServerがUSB接続のMCU-Linkを操作し、MCU-LinkがSWDというデバッグ用の接続でCPU0を操作する。GDBはELF内のソース位置・変数の情報を使い、LinkServerを通してブレークポイントやステップ実行、メモリ確認を行う。
3. **表示の確認**：CPU0で実行した`printf`のUART出力は、MCU-Linkの仮想COMポートを通ってTera Termに届く。今回のCOMポートはCOM5。Tera Termは出力を読む役割で、ビルドや書き込みは行わない。

<a id="qa-014-supplement"></a>

### QA-014：CMakeの相対ソースパスの基準

対象のCMakeLists.txtは次の場所にある。

```text
C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947\stages\02_led_control\CMakeLists.txt
```

このファイルを処理しているときの`CMAKE_CURRENT_SOURCE_DIR`は、その親フォルダの
`C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947\stages\02_led_control`。
`target_sources(app PRIVATE src/main.c)`の`src/main.c`は、この場所を基準に解釈される。

```text
基準：C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947\stages\02_led_control
相対指定：src/main.c
対象：C:\Users\grace\Documents\ChatGPT\zephyr_frdm_mcxn947\stages\02_led_control\src\main.c
```

これから実行する`west build`では、最後の引数に段階2のアプリフォルダを指定する。
westがそのフォルダをCMakeのソースディレクトリとして渡し、CMakeがそこでCMakeLists.txtを読む。
PowerShellの現在位置がZephyr本体であっても、アプリのソースディレクトリは別に指定できる。
`-d $ledBuild`で指定するビルド結果の保存先も、この相対ソースパスの基準とは別。

`app`はソースを追加するビルド対象名。`PRIVATE`は、このソースをapp自身のビルドに使い、
appに依存する他のビルド対象へソースとして引き継がせない指定。いずれもパスの基準を決めるものではない。

採用CMake 3.31.8の同梱Help（`command/target_sources.rst`、`variable/CMAKE_CURRENT_SOURCE_DIR.rst`、
`policy/CMP0076.rst`）を確認。対応する[公式説明](https://cmake.org/cmake/help/v3.31/command/target_sources.html)。
Codexの事前ビルド先`build/02_led_control_codex/build.ninja`でも、このmain.cの絶対パスを
コンパイル対象として確認した。この確認は本人のビルド結果としては扱わない。

<a id="qa-015-supplement"></a>

### QA-015：printfとprintkの比較

| 項目 | printf | printk |
|---|---|---|
| 提供元 | C標準ライブラリ（libc） | Zephyr |
| ヘッダ | `<stdio.h>` | `<zephyr/sys/printk.h>` |
| 出力先の考え方 | 標準出力stdout | Zephyrのシステムコンソール |
| 戻り値 | int。成功時は出力文字数、エラー時は負の値 | void。戻り値なし |
| 今回の用途 | 段階1のHello Worldで使用 | 段階2のGPIO APIの状態・戻り値を表示 |

```c
/* printfには<stdio.h>、printkには<zephyr/sys/printk.h>が必要。 */
printf("ret=%d\n", ret);
printk("ret=%d\n", ret);
```

今回の構成なら、どちらも同じUARTコンソールへ表示できる。`ret`はGPIO APIが返した値であり、
printkの戻り値ではない。表示するAPIを変えてもGPIOの論理値やLEDの極性は変わらない。

Codexの段階2の事前ビルドで生成した`.config`では、`CONFIG_PICOLIBC=y`、
`CONFIG_STDOUT_CONSOLE=y`、`CONFIG_PRINTK=y`、`CONFIG_UART_CONSOLE=y`を確認。
`CONFIG_LOG`は無効。採用ソースの`drivers/console/uart_console.c`は、stdout側とprintk側に
同じ`console_out`を登録する。`lib/libc/picolibc/stdio.c`はstdoutの出力先へ文字を渡す。
`lib/os/printk.c`はprintkの出力先へ文字を渡し、今回のPicolibc構成では書式処理にvfprintfを使う。

したがって「printkはいつも小さい」「printfは必ずバッファにためる」「printkでは特定の書式が
一律に使えない」という固定的な区別をしない。書式の対応、同期、ログ経由などは設定に依存する。
本人の今回の報告はソースの読み出し結果で、上の生成設定確認はCodexの事前検証を根拠としている。

参照（採用コミットに固定）：

- [printkの宣言](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/include/zephyr/sys/printk.h)
- [printkの実装](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/lib/os/printk.c)
- [UARTコンソールの出力先登録](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/drivers/console/uart_console.c)
- [Picolibcの標準出力接続](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/lib/libc/picolibc/stdio.c)
- [書式付き出力の説明](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/doc/services/formatted_output.rst)

<a id="qa-017-supplement"></a>

### QA-017：statusの有効化と実行時の状態

Devicetreeのstatusは、ハードウェアを表すノードを使用対象として有効にするかを記述する。

| 設定 | 意味 |
|---|---|
| `status = "okay"` | ノードを有効にする |
| `status = "disabled"` | ノードを明示的に無効にする |
| statusを省略 | Zephyrでは有効として扱う |

今回の共通定義はred_ledのstatusをdisabledにし、CPU0用の定義は同じノードを参照して
そのプロパティを上書きする。別の赤色LEDを追加する処理ではない。

```dts
&red_led {
    status = "okay";
};
```

これはCPU0向けの最終Devicetreeでred_ledを有効にする指定。電源投入後に実行されるCの命令ではない。

| 確認／処理 | 確認するもの、変えるもの |
|---|---|
| Devicetreeのstatus | ビルドへ入力するノードの有効化設定 |
| `gpio_is_ready_dt(&led)` | 実行時にgpio0デバイスの初期化が成功して使用可能か |
| `gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE)` | 実行時にピンを出力に設定し、初期状態を消灯へする |
| `gpio_pin_set_dt(&led, 1)` | 実行時に論理1を指定し、今回のActive Lowの赤色LEDを点灯させる |

ノードをokayにするだけで対応ドライバのコードが必ず組み込まれるわけではなく、Kconfigでの
ドライバ有効化なども必要。今回のred_ledはGPIO情報の取得先として使い、gpio0のドライバを
GPIO APIから操作する。GPIO情報の取得先が有効かをコンパイル時のDT_NODE_HAS_STATUSで確認し、
GPIOデバイスの使用可能状態を実行時のgpio_is_ready_dtで確認する。

okayは設定であり、LEDが実際に点灯したことや動作確認済みであることを表す値ではない。
実機結果は本人の確認を受けて別に記録する。

採用コミットの[Devicetreeのstatusの説明](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/doc/build/dts/intro-syntax-structure.rst)に照合済み。

<a id="qa-018-supplement"></a>

### QA-018：zephyr.dtsの入力・生成先・参照元

`zephyr.dts`は、ビルド時に決まったDevicetreeを一つにまとめた生成ファイル。
今回の生成先は`C:\Users\grace\zephyrproject-mcxn947\build\02_led_control\zephyr\zephyr.dts`。
`west build`の`-d $ledBuild`で指定したビルド先の下に作成され、学習アプリのソースフォルダとは別に置かれる。

今回の入力は、ボード指定`frdm_mcxn947/mcxn947/cpu0`で選ばれた
`C:\Users\grace\zephyrproject-mcxn947\zephyr\boards\nxp\frdm_mcxn947\frdm_mcxn947_mcxn947_cpu0.dts`と、
そこから読み込むCPU0用・共通ボード・SoCなどの`.dtsi`。
overlayを使う場合はその上書きも反映されるが、今回のアプリにはoverlayを追加していない。
`prj.conf`はKconfigへの入力で、`zephyr.dts`へ直接統合する入力ではない。

採用版4.4.2の実際の生成処理：

```mermaid
flowchart TD
    A[ボード .dts / .dtsi / 必要なら overlay] --> B[前処理: zephyr.dts.pre]
    Y[bindings: プロパティの意味を定める YAML] --> C[gen_edt.py による解析]
    B --> C
    C --> D[zephyr.dts: 統合後の設定を確認]
    C --> E[edt.pickle: 解析結果]
    E --> F[gen_defines.py]
    F --> G[devicetree_generated.h: C用マクロ]
    G --> H[Zephyrのヘッダ経由で main.c / ドライバをコンパイル]
    D --> I[DTCによる追加の警告・エラー検査]
```

`zephyr.dts`とC用の生成ヘッダは、同じ解析結果から作られる。
採用版では、生成済み`zephyr.dts`を読み直してヘッダを作る経路ではない。
`cmake/modules/dts.cmake`がこの処理を実行し、DTCの検査出力自体は破棄する。

C側は`<zephyr/devicetree.h>`などの通常のZephyrヘッダを経由して生成マクロを使う。
アプリが`devicetree_generated.h`を直接includeする必要はない。
今回の`main.c`は`<zephyr/drivers/gpio.h>`を読み込み、その依存ヘッダを経由してDevicetreeのAPIを使っている。

```c
#define LED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
```

この記述は、ビルド時に決まった`led0 → red_led → gpio0・ピン10・GPIO_ACTIVE_LOW`の情報を
`led`の初期値に取り込む。実行時にボードが`zephyr.dts`を開いて読む処理ではない。

`zephyr.dts`の役割は、複数の入力の上書きを経た最終設定を確認し、設定の問題を調べること。
たとえばred_ledの`status = "okay"`を見れば、共通定義のdisabledがCPU0用定義で上書きされたことを確認できる。
ビルドで再生成されるため、設定変更は入力のoverlayなどへ行い、生成ファイルを直接編集しない。

参照：採用コミットの[生成処理](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/cmake/modules/dts.cmake)、
[gen_edt.py](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/scripts/dts/gen_edt.py)、
[DevicetreeのC用ヘッダ](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/include/zephyr/devicetree.h)。
公式の[入力・出力ファイルの説明](https://docs.zephyrproject.org/latest/build/dts/intro-input-output.html)も参照し、スクリプト間の分担はローカルの採用版ソースで照合した。

<a id="qa-019-supplement"></a>

### QA-019：overlayを使う場面と今回不要な理由

overlayは、ボードの`.dts`／`.dtsi`を直接編集せずに、アプリに合わせてDevicetreeへ設定を追加・上書きする入力ファイル。
拡張子は通常`.overlay`で、内容はDevicetreeの記述。ビルド時にボード定義の後へ適用され、最終`zephyr.dts`とC用生成ヘッダへ反映される。
実行中に動的に設定を重ねる処理ではない。

判断の基準は「このアプリで必要なハードウェア構成が、選んだボードの既定Devicetreeで表現されているか」。

| やりたいこと・現状 | 設定する場所 |
|---|---|
| 既定のLEDが有効で、ピン・極性・aliasも目的に合っている | 追加overlayなしで既存の情報を使う |
| 使いたい周辺機器が最終Devicetreeでdisabled | overlayで有効化し、必要なピン設定なども確認。ドライバのKconfigも別途確認 |
| 外付けセンサーなど、使うデバイスのノードがない | overlayへ対応ノードと、接続先・アドレスなどを追加 |
| Cコードを変えず、led0などのaliasの割り当てを変えたい | overlayでaliasを上書き |
| GPIO APIやprintkなどのソフトウェア機能を組み込みたい | prj.confでKconfigを設定 |
| 同じLEDを点灯から消灯へ変えたい | main.cの論理値を変更 |

実際の回路・配線とアプリの要件を確認し、対象ボードで生成した`zephyr.dts`と比較する。
変更するDevicetreeプロパティの意味・必要な項目は対応するbindingも確認する。
`status = "okay"`だけで、必要なドライバの組み込みや配線条件まで満たすとは判断しない。

今回の照合結果：

| 必要な情報 | 既定のボード定義と本人の確認結果 |
|---|---|
| 操作するLED | `led0 = &red_led`で赤色LEDを参照できる |
| LEDノードの有効化 | 共通定義ではdisabledだが、CPU0用定義でred_ledがokayになる |
| GPIOデバイスの有効化 | CPU0用定義でgpio0がokayになる |
| ピン・極性 | `gpios = <&gpio0 10 GPIO_ACTIVE_LOW>`がオンボード赤色LEDの制御に合っている |
| 出力・ピンmuxの設定 | 採用版のMCUX GPIOドライバがgpio_pin_configure_dt経由で設定。LED用の追加pinctrl指定は不要 |
| GPIOドライバ | prj.confでGPIOを要求し、本人の.configでGPIOとGPIO_MCUXがy |

このため追加overlayなしで目的を満たせると判断した。生成設定の確認に続き、本人のUARTで準備状態trueと両APIの戻り値0、目視で点灯・消灯も確認済み。

同じCコードの`DT_ALIAS(led0)`を維持したまま、led0を緑色LEDへ割り当てる場合のoverlayの記述例：

```dts
/ {
    aliases {
        led0 = &green_led;
    };
};
```

採用したCPU0用定義ではgreen_ledもすでにokay。これは設定変更の説明用であり、実際のサンプルには追加していない。
元のCコードにある赤色LED向けの表示文は、緑色LEDへ変更するアプリでは合わせて更新する。

参照：[公式のoverlayの使い方](https://docs.zephyrproject.org/latest/build/dts/howtos.html#set-devicetree-overlays)、
採用コミットの[共通ボード定義](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/boards/nxp/frdm_mcxn947/frdm_mcxn947.dtsi)、
[CPU0用定義](https://github.com/zephyrproject-rtos/zephyr/blob/dccb09599635bdff17633fa7e9dab014b91dce90/boards/nxp/frdm_mcxn947/frdm_mcxn947_mcxn947_cpu0.dtsi)。

## 段階6の開始時の説明

| ID | 質問した段階 | 質問日時（JST、代替日時は注記） | 質問のカテゴリ | 質問内容 | 回答 |
|---|---|---|---|---|---|
| QA-037 | 段階6／#7 | 2026-10-14（引き継ぎ依頼の受領日。正確な質問日時は不明） | UART受信API・ISR | 採用版でuart_poll_inの戻り値と入力なし時の動作、割り込みコールバックの実行場所、ISRとスレッドの役割を説明してください。 | uart_poll_inは非ブロッキングで、1バイトを保存できれば0、入力が空なら-1。成功時だけ保存先を使う。採用MCUX LPUART実装はフラグを1回確認する。空のときのk_msleep(1)はアプリ側の待機。割り込みコールバックはドライバISRから呼ばれるためISRの文脈で動く。FIFO読出し、上限・改行判定、K_NO_WAITのキュー投入を短く行い、待ち付きのキュー受取りとUART出力はスレッド側へ置く。採用コミットのローカルソースによる説明であり、本人の理解・実機結果の確認ではない。[段階6の準備記録](../stages/06_uart_echo/README.md)。 |

## 段階6：割り込み受信の図解

| ID | 質問した段階 | 質問日時（JST、代替日時は注記） | 質問のカテゴリ | 質問内容 | 回答 |
|---|---|---|---|---|---|
| QA-038 | 段階6／#7 | 2026-10-24（質問受領日。正確な送信日時は未取得） | UART割り込み・ISR・メッセージキュー | 割り込み版の動作メカニズムが分からないので、ISR側でキューの空きを待たない理由を図解で教えてください。 | CPU0は1個で、ISRは実行中のスレッドへ割り込んで動く。UARTから読んだ文字をISR内で行へ組み立て、完成した行を待たずにキューへコピーして戻る。ISR終了後にmainが行を取り出して送信する。ISRがキューの空きを待って回り続けると、空きを作るmainが動けず進まない。Zephyrの採用実装でもISRからのk_msgq_putはK_NO_WAITだけを許す。今回は満杯なら新しい完成行を捨て、mainが後で通知する。[以下の図と具体例](#qa-038-supplement)を参照。説明後の本人の理解確認はまだ行っていない。 |

<a id="qa-038-supplement"></a>

CPU0は1個で、mainとLEDスレッドはCPU時間を分けて動く。ISR（割り込み処理）は別スレッドではなく、
実行中のスレッドを一時中断して優先的に行う処理。UART回路とRAMのキュー自体がCの処理を実行するわけではない。

次の図は、abcと改行を受け取る複数回の割り込みをまとめて示している。

```mermaid
sequenceDiagram
    participant P as PC
    participant U as UART回路
    participant C as CPU0
    participant Q as RAMのキュー
    Note over C: mainは行待ち<br/>LEDは点滅して待機
    P->>U: abc と改行を送る
    U-->>C: 受信割り込み
    Note over C: スレッドを一時中断しISRを実行
    C->>U: ISRが到着した文字を読む
    C->>Q: ISRが完成した行をコピー
    Note over C: ISR終了・スレッド実行へ戻る
    C->>Q: mainが1行取り出す
    C->>U: mainがEcho: abcを送る
    U-->>P: Echo: abc
```

改行前はISRが本文を蓄積するだけで、まだキューへ完成行を渡さない。
改行が届いたら`k_msgq_put(..., K_NO_WAIT)`でコピーし、ISRは終了する。
mainの`k_msgq_get(..., K_FOREVER)`は行がない間mainを待ち状態にし、受信した行を送信できる状態になるとmainを実行可能にする。
mainが待っている間、LEDスレッドが実行可能なら動ける。両方が待ち状態ならCPUはアイドルにも入れる。
割り込みで起こされたmainが、そのISRの途中で実行を始めるわけではない。

キューが10行で満杯のとき、ISR内で「空くまで回り続ける」処理を書いたと仮定すると、次の循環になる。

```mermaid
flowchart TD
    A["ISRがキューの空きを待って回り続ける"] --> B["ISRが終了しない"]
    B --> C["CPU0でmainが動けない"]
    C --> D["mainが行を取り出せず空きができない"]
    D --> A
```

これは待ち続ける処理が不適切な理由の説明であり、ISRで待機APIを使う実装例ではない。
採用Zephyrの`kernel/msg_q.c`のput_msg_in_queueでは、ISRからの呼出しにK_NO_WAITを要求するassert条件を確認した。
実際のアプリは「入るなら投入、満杯なら新しい完成行を捨てて戻る」とし、mainが再開してから通知する。
捨てる単位は今回完成した1行であり、すでにキューにある10行は残す。

今回の001〜100が全部返った理由は、mainも途中で行を取り出して送信するため。
キュー10件は同時に溜めておける上限であり、合計10行しか処理できない意味でも、10行揃うまで待つ意味でもない。
今回の試験では満杯通知や欠番は観測されず、キュー満杯経路の実機確認は未実施として区別する。
PC側の送信間隔や最大キュー占有数は測定していないため、満杯にならなかった細かい時間条件は断定しない。

## 段階6：実際の関数名・変数名で見る受信経路

| ID | 質問した段階 | 質問日時（JST、代替日時は注記） | 質問のカテゴリ | 質問内容 | 回答 |
|---|---|---|---|---|---|
| QA-039 | 段階6／#7 | 2026-10-24（質問受領日。正確な送信日時は未取得） | UART割り込み・ソース読解 | 提示した図を、ソースコードの実際の名称でもう一歩具体的に説明してください。 | irq_line.cのmainがserial_cbを登録し、UART割り込みを有効にする。ISR文脈のserial_cbはuart_fifo_readでcへ読み、receive_byte(c)を呼ぶ。receive_byteがrx_line.textとrx_lenを更新し、改行でrx_lineをrx_queueへコピーする。mainはk_msgq_getで自分のmessageへ受け取り、send_message、uart_send_string、uart_poll_outの順で送信する。rx_lineとmainのmessageは別の構造体。満杯時はqueue_overflowを立ててISRから戻り、mainが後で通知する。[以下の図・abcの具体例](#qa-039-supplement)を参照。 |

<a id="qa-039-supplement"></a>

対象：[割り込み版のソース](../stages/06_uart_echo/src/irq_line.c)。図は正常な本文・改行の経路を中心に示す。
serial_cbはmainからUARTドライバへ登録するコールバック。受信ごとにmainが直接呼ぶ関数ではない。

```c
uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);
uart_irq_rx_enable(uart_dev);
uart_irq_err_enable(uart_dev);
```

受信割り込みが来ると、採用ドライバのISRからserial_cbが呼ばれる。
serial_cbから呼ぶreceive_byteとrecord_rx_errorsも、その呼出し中はISR文脈で実行される。

```mermaid
flowchart TD
    A["UART受信割り込み"] --> B["serial_cb(dev, user_data)"]
    B --> C["uart_fifo_read(dev, &c, 1)<br/>cへ1バイトを読む"]
    C --> D["receive_byte(c)"]
    D -->|"本文"| E["rx_line.text[rx_len++]へ格納"]
    D -->|"CRまたはLF"| F["rx_line.textをNUL終端"]
    F --> G["k_msgq_put(&rx_queue, &rx_line, K_NO_WAIT)"]
    E --> H["次の到着済みバイトを読む<br/>FIFOが空ならserial_cbから戻る"]
    G --> H
```

実際のserial_cbはこの読出し前にuart_irq_update、record_rx_errors、uart_irq_rx_readyを呼ぶ。
whileでFIFOが空になるまで読むため、割り込み1回で複数バイトを読む場合もある。
CR直後のLFはprevious_was_crの判定で無視し、完成行を二重に投入しない。

最初の正常行としてabcとCRLFを受信する例：

| c | receive_byte(c)の主な処理 | 処理後のrx_len |
|---|---|---:|
| a | rx_line.text[0]へaを格納 | 1 |
| b | rx_line.text[1]へbを格納 | 2 |
| c | rx_line.text[2]へcを格納 | 3 |
| CR | rx_line.text[3]をNUL終端し、rx_lineをrx_queueへコピー。長さと状態を初期化 | 0 |
| LF | previous_was_crがtrueなのでreturn。2件目を投入しない | 0 |

行終端で配列全体を消す処理はしていない。長さと結果・UARTエラーフラグを初期化し、次の本文で上書きする。
キューへはstruct line_messageの内容がコピーされるため、その後rx_lineを更新しても渡した行は変わらない。

```mermaid
flowchart TD
    Q["rx_queue<br/>struct line_messageを最大10件保持"] --> G["mainのk_msgq_get<br/>messageへ1件受け取る"]
    G --> S["send_message(&message)"]
    S --> U["uart_send_string<br/>Echo:・本文・CRLFを順に送る"]
    U --> O["uart_poll_out(uart_dev, ...)<br/>1バイトずつ送信"]
    O --> P["PCにEcho: abcを表示"]
```

mainの実際の受取りは`k_msgq_get(&rx_queue, &message, K_FOREVER)`。
キューが空ならmainだけが待ち状態になる。受取り後、send_message(&message)へ進む。
mainがすでに受取り待ちなら、カーネルはメッセージを待ち先のmessageへ直接コピーして起こすこともある。
図はメッセージキューAPIを介した受渡しを示し、必ずキューの内部バッファに一度溜まるという意味ではない。

| 名前 | 内容と担当 |
|---|---|
| serial_cb内のc | UARTから読んだ1バイト |
| rx_line | ISR側の組立て用struct line_message。text・result・uart_errorsを持つ |
| rx_len | ISR側で本文を何バイト蓄積したか |
| rx_queue | 完成したメッセージを受け渡すキュー |
| main内のmessage | mainが受け取る別のstruct line_message |
| send_message内のmessage | mainから渡された構造体へのポインタ。message->textで本文を読む |

キュー満杯ではreceive_byteのk_msgq_putが失敗し、atomic_set(&queue_overflow, 1)を行う。
その行は捨ててserial_cbから戻り、mainが再び動けるようにする。
mainは行を受け取った後、atomic_clear(&queue_overflow)の戻り値で通知の有無を確認し、エラー表示する。
receive_byteが空くまで回り続けるとserial_cbが戻れず、空きを作るmainのk_msgq_getも進めない。
そのためISR側はK_NO_WAITで投入する。これは前の一般図を実際の関数名へ対応づけた説明である。

led_entryはk_thread_createで作る別スレッドで、gpio_pin_set_dtとk_msleep(250)を繰り返す。
serial_cbが戻り、LEDスレッドが実行可能なら点滅処理が動く。LED側はUARTへ送信しない。

## 関連する記録

- [学習ロードマップ](roadmap.md)
- [段階0の選定記録](../stages/00_versions/versions.md)
- [段階1の手順と本人の実習結果](../stages/01_hello_world/README.md)
