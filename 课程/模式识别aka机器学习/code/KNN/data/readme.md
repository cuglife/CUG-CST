每个属性含义，以第一个数据为例：

@relation 'autoPrice.names'



1. @attribute symboling real   
   * symboling: 2
   * 精算师称这一过程为“象征”。值为+3表示汽车有风险，-3表示它可能相当安全。
   * 保险风险评级：(-3, -2, -1, 0, 1, 2, 3).
2. @attribute normalized-losses real 
   * normalized-losses: 164
   * 每辆保险车辆年平均相对损失支付.
   * 连续65到256。

3. @attribute wheel-base real
   * wheel-base: 99.8
   * 轴距
   * continuous from 86.6 120.9.

4. @attribute length real 
   * length: 176.6,
   * 汽车长度
   * continuous from 141.1 to 208.1.

5. @attribute width real 
   * width: 66.2,
   * 汽车宽度
   * continuous from 60.3 to 72.3.

6. @attribute height real 

   * height: 54.3,
   * 汽车高度
   * continuous from 47.8 to 59.8.
7. @attribute curb-weight real 
   * curb-weight: 2337
   * 车体自重
   * continuous from 1488 to 4066.
8. @attribute engine-size real 
   * engine-size: 109
   * 发动机尺寸
   * continuous from 61 to 326.
9. @attribute stroke real 
   * stroke: 3.19
   * 行程？
   * continuous from 2.07 to 4.17.
10. @attribute bore real 
    * 3.4
    * continuous from 2.54 to 3.94.
11. @attribute compression-ratio real 
    * compression-ratio: 10
    * 燃气压缩比？
    * continuous from 7 to 23.
12. @attribute horsepower real 
    * horsepower: 102
    * 马力
    * continuous from 48 to 288.
13. @attribute peak-rpm real 
    * rpm: 5500
    * 峰值转速
    * continuous from 4150 to 6600.
14. @attribute city-mpg real 
    * 24
    * 市区汽车的燃油性能，mpg：每加仑英里数
    * continuous from 13 to 49.
15. @attribute highway-mpg real 
    * 30
    * 高速公路汽车的燃油性能，mpg：每加仑英里数
    * continuous from 16 to 54.
16. @attribute class real
    * price：13950
    * 价格
    * continuous from 5118 to 45400.





|                                                              |                          数据集简介                          |                                                              |
| :----------------------------------------------------------: | :----------------------------------------------------------: | :----------------------------------------------------------: |
|                       主要包括3类指标:                       |                           类别属性                           |                           连续指标                           |
| 汽车的各种特性. 保险风险评级：(-3, -2, -1, 0, 1, 2, 3). 每辆保险车辆年平均相对损失支付. | make: 汽车的商标（奥迪，宝马。。。）fuel-type: 汽油还是天然气<br/>aspiration: 涡轮<br/>num-of-doors: 两门还是四门<br/>body-style: 硬顶车、轿车、掀背车、敞篷车<br/>drive-wheels: 驱动轮<br/>engine-location: 发动机位置<br/>engine-type: 发动机类型<br/>num-of-cylinders: 几个气缸<br/>fuel-system: 燃油系统<br/> | bore: continuous from 2.54 to 3.94.stroke: continuous from 2.07 to 4.17.<br/>compression-ratio: continuous from 7 to 23.<br/>horsepower: continuous from 48 to 288.<br/>peak-rpm: continuous from 4150 to 6600.<br/>city-mpg: continuous from 13 to 49.<br/>highway-mpg: continuous from 16 to 54.<br/>price: continuous from 5118 to 45400.<br/> |

