# coding: utf-8
# %load                  TC_J
from pynverse import inversefunc
import pandas as pd
import numpy as np
c0, c1, c2, c3, c4, c5, c6, c7, c8 = (
    0,
    0.503811878150E-01,
    0.304758369300E-04,
    -0.856810657200E-07,
    0.132281952950E-09,
    -0.170529583370E-12,
    0.209480906970E-15,
    -0.125383953360E-18,
    0.156317256970E-22
    )
c00, c01, c02, c03, c04, c05 = (
0.296456256810E+03,
-0.149761277860E+01,
 0.317871039240E-02,
-0.318476867010E-05,
 0.157208190040E-08,
-0.306913690560E-12
)
JmV = lambda T: c0+c1*T+c2*T**2+c3*T**3+c4*T**4+c5*T**5+c6*T**6+c7*T**7+c8*T**8 if T <= 760 else c00+c01*T+c02*T**2+c03*T**3+c04*T**4+c05*T**5
y = np.arange(-8, 71, 2)
digit=1
tab_T = [round(inversefunc(JmV, y_values=mV).item(0), digit)*(10**digit) for mV in y]
print(inversefunc(JmV, y_values=29.46142290433).round(digit)*(10**digit))
print(inversefunc(JmV, y_values=38.24024176).round(digit)*(10**digit))
df = pd.DataFrame({"temperature": tab_T, "mV": y})
df.to_csv("TC-J.csv")
