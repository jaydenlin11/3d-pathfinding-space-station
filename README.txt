# personalization-attack

Installation
---
Assuming you have Miniconda installed
```shell
conda create -n attack python=3.9
conda activate attack
pip install -r requirements.txt
```

Quick start
---
## Text Inversion

```shell
python textual_inversion_train.py
python textual_inversion_generate.py
```

Reproduce results in YH2024
---
```shell
bash reproduce-YH2024.sh
```


