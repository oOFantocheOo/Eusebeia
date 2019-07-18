// 模型中单位统一采取厘米
// 每单位长度表示一厘米

EPS = 0.01;

BodyHeight = 80;  // 产品高度
BodyRadius = 30;  // 产品外径
ShellThickness = 1;  // 板材厚度
UmbrellaRadius = 12.5;  // 伞孔半径

DrawerWindowHeight = 60;  // 抽屉窗口高度
DrawerWindowWidth = 15;  // 抽屉窗口宽度

// 抽屉长宽高
DrawerLenth = 12.5;
DrawerWidth =
  DrawerWindowWidth-EPS*2;
DrawerHeight =
  DrawerWindowHeight-EPS*2;

// 伞格长宽高
UmbrellaBoxLenth =
  DrawerLenth;
UmbrellaBoxWidth =
  DrawerWidth - ShellThickness*2;
UmbrellaBoxHeight = 45;

// 抽屉数量
DrawerNumber = 6;


// 别名
// Do not edit these names
BH = BodyHeight;
BR = BodyRadius;
ST = ShellThickness;
UR = UmbrellaRadius;

DWH = DrawerWindowHeight;
DWW = DrawerWindowWidth;

DL = DrawerLenth;
DW = DrawerWidth;
DH = DrawerHeight;

UBL = UmbrellaBoxLenth;
UBW = UmbrellaBoxWidth;
UBH = UmbrellaBoxHeight;

DN = DrawerNumber;


// 产品外壳
module Body() {
  difference() {
    cylinder(h=BH, r=BR);
  
    // 中间空心
    translate([0, 0, ST]) {
      cylinder(
        h=BH - 2*ST,
        r=BR-ST
      );
    }
  
    // 伞孔
    translate([0, 0, BH-ST-EPS]) {
      cylinder(
        h=ST+EPS*2,
        r=UR
      );
    }
  
    // 抽屉窗口
    translate([BR, 0, ST*2+DWH/2]) {
      cube(
        [BR, DWW, DWH],
        center=true
      );
    }
  }
}


// 抽屉模型
module Drawer() {
  difference() {
    cube([
      DL,
      DW,
      DH
    ]);
    translate([
      ST, ST, UBH+ST*2
    ]) {
      cube([
        DL-ST*2,
        DW-ST*2,
        DH-UBH-ST*2+EPS
      ]);
    }
    translate([
      -EPS/2, ST, ST
    ]) {
      cube([
        UBL+EPS,
        UBW,
        UBH
      ]);
    }
  }
}


// Start here
DrawPostionOffset = [
  BR-DL-ST*2,
  -DW/2,
  ST*2,
];

DPO = DrawPostionOffset;


Body();

color("blue") {
  translate(DPO) {
    Drawer();
  }
  for (i = [1:DN]) {
    rotate([0, 0, i*(360/DN)]) {
      translate(DPO) {
        Drawer();
      }
    }
  }
}

