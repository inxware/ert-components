# coding: utf-8
from pynverse import inversefunc
import pandas as pd
import numpy as np
R = lambda T: 100 * (1 + 3.9083e-3 * T - 5.775e-7 * T**2) if T >= 0 else 100 * (1 + 3.9083e-3 * T - 5.775e-7 * T**2 - 4.183e-12 * T**3 * (T-100))
y = np.arange(18, 405, 3)
digit=1
tab_T = [int(round(inversefunc(R, y_values=r).item(0), digit)*(10**digit)) for r in y]
df = pd.DataFrame({"temperature": tab_T, "resistance": y})
df.to_csv("PT-100.csv")
