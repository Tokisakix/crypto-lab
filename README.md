# SYSU 2024 Crypto

## 现代密码学

`example` 文件夹则存放了中山大学 2024 年《现代密码学》重要算法的 Python 实现，仅供参考

已实现的算法有

- [x] 维吉尼亚密码
- [x] 希尔密码
- [x] DES 加密
- [x] AES 加密
- [x] MD5 哈希
- [x] SHA-1 哈希
- [x] RSA 体制
- [x] ElGamal 体制

**如何运行**

```bash
pip install -r requirements.txt
cd example && python main.py --type <Crypto-Algorithm-Name>
```

其中，`<Crypto-Algorithm-Name>` 指定了使用的算法，目前支持的选项有 `vigenere`、`hill`、`des`、`aes`、`md5`、`sha1`、`rsa`、`elgamal`

## 现代密码学实验

此仓库还有中山大学 2024 秋季学期《现代密码学实验》的全 AC **佛系**实现，笔者并没有特意做过多的优化，仅供参考