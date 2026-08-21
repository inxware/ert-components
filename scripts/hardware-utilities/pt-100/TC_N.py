# coding: utf-8
from pynverse import inversefunc
import pandas as pd
import numpy as np
c0, c1, c2, c3, c4, c5, c6, c7, c8 = (
 0.000000000000E+00,
 0.261591059620E-01,
 0.109574842280E-04,
-0.938411115540E-07,
-0.464120397590E-10,
-0.263033577160E-11,
-0.226534380030E-13,
-0.760893007910E-16,
-0.934196678350E-19
)
c00, c01, c02, c03, c04, c05, c06, c07, c08, c09, c010 = (
 0.000000000000E+00,
 0.259293946010E-01,
 0.157101418800E-04,
 0.438256272370E-07,
-0.252611697940E-09,
 0.643118193390E-12,
-0.100634715190E-14,
 0.997453389920E-18,
-0.608632456070E-21,
 0.208492293390E-24,
-0.306821961510E-28
)
NmV = lambda T: c0+c1*T+c2*T**2+c3*T**3+c4*T**4+c5*T**5+c6*T**6+c7*T**7+c8*T**8 if T < 0 else c00+c01*T+c02*T**2+c03*T**3+c04*T**4+c05*T**5+c06*T**6+c07*T**7+c08*T**8+c09*T**9+c010*T**10
y = np.arange(-4, 47, 1)
digit=1
tab_T = [int(round(inversefunc(NmV, y_values=mV).item(0), digit)*(10**digit)) for mV in y]
print(round(inversefunc(NmV, y_values=26.07398373).item(0), digit)*(10**digit))
print(round(inversefunc(NmV, y_values=33.3697784244).item(0), digit)*(10**digit))
df = pd.DataFrame({"temperature": tab_T, "mV": y})
df.to_csv("TC-N.csv")
