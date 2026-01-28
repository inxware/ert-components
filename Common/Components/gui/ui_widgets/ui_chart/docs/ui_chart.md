ui_chart
User interface plot chart widget
Menu: 
            User Interface
             > Graphics > Advanced > Chart
Type: IO

            ┌─────────────────────────┐
    create►─┤                         ├►─-- 
   destroy►─┤                         ├►─-- 
      show►─┤                         ├►─-- 
      hide►─┤                         ├►─-- 
    update►─┤                         ├►─-- 
     off.x──┤                         ├──abs x (I)
     off.y──┤                         ├──abs y (I)
   off.wid──┤                         ├──wid (I)
    off.ht──┤                         ├──ht (I)
     alpha──┤                         │
    update►─┤                         ├►─-- 
            │          Chart          │
      minx──┤                         │
      maxx──┤                         │
      miny──┤                         │
      maxy──┤                         │
      auto──┤                         │
      data►─┤                         ├►─-- 
     data1──┤                         │
     data2──┤                         ├►─click 
     data3──┤                         ├►─mouse down 
     data1──┤                         │
     data2──┤                         │
     data3──┤                         │
            └─────────────────────────┘

Legend: ── Data | ►─ Event

**Parameters (14):**
- **Widget Tag:** widget - widget tag
- **Widget-Group:** default - widget group
- **Data Size:** 10 (0 to 1000) - Number of data points
- **X Max:** 10 (-65536 to 65536) - Maximum x-axis range value
- **X Min:** 0 (-65536 to 65536) - Minimum x-axis range value
- **Y Max:** 100 (-65536 to 65536) - Maximum y-axis range value
- **Y Min:** 0 (-65536 to 65536) - Minimum y-axis range value
- **Horizontal lines:** 5 (0 to 20) - Number of horizontal division lines.
- **Vertical lines:** 9 (0 to 20) - Number of vertical division lines.
- **Auto range:** 0 (Bool) - Adjusts y-axis range based on data min max values 
- **NaN:** -32768 (-32768 to 32768) - Number used for identifying NaN value
- **Enable Plot 1:** 1 (Bool) - Creates data series for plot 1
- **Enable Plot 2:** 1 (Bool) - Creates data series for plot 2
- **Enable Plot 3:** 0 (Bool) - Creates data series for plot 3

**Port Summary:**
- **Left:**  23 ports (7 events, 16 data)
- **Right:** 13 ports (9 events, 4 data)
