# 段階0：使用バージョンと開発環境の方針

## 目的

後続の段階で同じビルド・検証を再現できるように、ツール、依存関係、接続方法を決めます。この段階ではサンプル実装を行いません。

## 作業内容

- [ ] Windowsネイティブ環境を基本として、使用するエディタとデバッグ手段を決める。
- [ ] FRDM-MCXN947、LCD_PAR_S035、OV7670への対応を確認してZephyrのタグ／コミットを選ぶ。
- [ ] Zephyr SDK、Python、west、CMake、Ninja、LinkServerの使用バージョンを決める。
- [ ] 段階14でSlintとZephyrの互換性を再確認する方針を記録する。
- [ ] stages/00_versions/versions.mdに選定理由、公式資料、依存モジュール固定方法を記録する。

## 完了条件

- [ ] 各ツールとZephyrのバージョン、および選定理由が記録されている。
- [ ] 必要機材・接続方法・環境構築方針が記録されている。
- [ ] 後の段階でバージョンを変更するときの記録方針が決まっている。

## 成果物

- [versions.md](versions.md)：選定したバージョン、理由、接続方法、変更方針。
- [対応するGitHubイシュー #1](https://github.com/grace2riku/zephyr_frdm_mcxn947/issues/1)：環境を選定した根拠と確認結果。

## 参考資料

- [FRDM-MCXN947](https://docs.zephyrproject.org/latest/boards/nxp/frdm_mcxn947/doc/index.html)
- [Getting Started](https://docs.zephyrproject.org/latest/develop/getting_started/)
