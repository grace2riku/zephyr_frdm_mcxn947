# 段階9のNT-Shell固定情報

2026-11-01（JST）、段階8の公式配布NT-Shell 0.3.1を本段階へバイト単位でコピーした。
段階9は独立してビルドでき、以前の段階のソースをビルド時に参照しない。

- 作者：Shinichiro Nakamura / CuBeatSystems。
- 公式配布物：[ntshell-v0.3.1.tar.gz](https://cubeatsystems.com/ntshell/resources/ntshell-v0.3.1.tar.gz)。
- SHA-256：`ea1497ce208cf9e3d02c161fd5d72e0e785b34074e31dca34fe855ba8fd89c74`。
- core全14ファイルとutil/ntopt.c・ntopt.hの計16ファイル。上流変更なし。
- MIT許諾・著作権表示、vtrecv.cのpublic domain表示を保持。
- ntconf.h未変更。編集領域64 B、履歴8件。
- 配布物は0.3.1だがntshell_version()は0.2.0。表示はpackage 0.3.1。
- Codexが既存の公式アーカイブと本段階の16ファイル・SHA256SUMSを照合済み。

```powershell
python "$learningRepo\stages\09_register_access\scripts\verify_vendor.py" `
    --archive "$learningRepo\.tmp\ntshell-v0.3.1.tar.gz"
```

元の取得経緯は[段階7](../07_ntshell/UPSTREAM.md)を参照。
