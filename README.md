# 電波天文学特論IV レポート問題1 提出
- plot.ipynb : 画像タイプ("PointSource", "2DGaussian"など)を指定し, C++のコードを実行. 結果をプロット
- CreatImage.cpp : plot.ipynbから画像タイプを受け取り, 256×256画像を作成.  image.txtを出力.
- Improved_FT.cpp : image.txtを読み込み, フーリエ変換を計算. その振幅画像をamplitude.txtとして出力.
- RadialProfile.cpp : amplitude.txtを読み込み, 振幅をrの関数として再編, r_amp.txtとして出力. 

追記
- plot.ipynbの実行には約20minかかります
