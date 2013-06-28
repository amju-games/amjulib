//Maya ASCII 2012 scene
//Name: loc1.ma
//Last modified: Sun, May 19, 2013 08:24:21 AM
//Codeset: UTF-8
requires maya "2012";
currentUnit -l centimeter -a degree -t film;
fileInfo "application" "maya";
fileInfo "product" "Maya 2012";
fileInfo "version" "2012 x64";
fileInfo "cutIdentifier" "201103110020-796618";
fileInfo "osv" "Mac OS X 10.8.2";
fileInfo "license" "student";
createNode transform -s -n "persp";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 2111.630431405456 1712.4840847106366 317.60614450964215 ;
	setAttr ".r" -type "double3" -80.138352729602644 0.60000000000024589 -9.9397783839289504e-17 ;
createNode camera -s -n "perspShape" -p "persp";
	setAttr -k off ".v" no;
	setAttr ".fl" 34.999999999999986;
	setAttr ".coi" 1675.154861389108;
	setAttr ".imn" -type "string" "persp";
	setAttr ".den" -type "string" "persp_depth";
	setAttr ".man" -type "string" "persp_mask";
	setAttr ".tp" -type "double3" -800.55276489257812 25.843488723039627 226.01315307617188 ;
	setAttr ".hc" -type "string" "viewSet -p %camera";
createNode transform -s -n "top";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 773.25516861160099 860.99659097891356 257.97014613251952 ;
	setAttr ".r" -type "double3" -89.999999999999986 0 0 ;
	setAttr ".rp" -type "double3" 1.1368683772161603e-13 3.5527136788005009e-15 -1.1368683772161603e-13 ;
	setAttr ".rpt" -type "double3" 0 -1.1723955140041656e-13 1.1013412404281583e-13 ;
createNode camera -s -n "topShape" -p "top";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.10000000000002;
	setAttr ".ow" 7030.3905233546484;
	setAttr ".imn" -type "string" "top";
	setAttr ".den" -type "string" "top_depth";
	setAttr ".man" -type "string" "top_mask";
	setAttr ".tp" -type "double3" -800.55276489257812 760.89659097891354 226.01315307617205 ;
	setAttr ".hc" -type "string" "viewSet -t %camera";
	setAttr ".o" yes;
createNode transform -s -n "front";
	setAttr ".v" no;
	setAttr ".t" -type "double3" -139.96028404365541 0 414.27627540012622 ;
createNode camera -s -n "frontShape" -p "front";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 4394.7094349080935;
	setAttr ".imn" -type "string" "front";
	setAttr ".den" -type "string" "front_depth";
	setAttr ".man" -type "string" "front_mask";
	setAttr ".hc" -type "string" "viewSet -f %camera";
	setAttr ".o" yes;
createNode transform -s -n "side";
	setAttr ".v" no;
	setAttr ".t" -type "double3" 189.75305408289603 0 226.01315307617196 ;
	setAttr ".r" -type "double3" 0 89.999999999999986 0 ;
createNode camera -s -n "sideShape" -p "side";
	setAttr -k off ".v" no;
	setAttr ".rnd" no;
	setAttr ".coi" 100.1;
	setAttr ".ow" 3060.8631415116156;
	setAttr ".imn" -type "string" "side";
	setAttr ".den" -type "string" "side_depth";
	setAttr ".man" -type "string" "side_mask";
	setAttr ".hc" -type "string" "viewSet -s %camera";
	setAttr ".o" yes;
createNode transform -n "loc1:polySurface1";
createNode transform -n "polySurface1" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape1" -p "polySurface1";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface2" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape2" -p "polySurface2";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface3" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape3" -p "polySurface3";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface4" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape4" -p "polySurface4";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface5" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape5" -p "polySurface5";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface6" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape6" -p "polySurface6";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface7" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape7" -p "polySurface7";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface8" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape8" -p "polySurface8";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface9" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape9" -p "polySurface9";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface10" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape10" -p "polySurface10";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface11" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape11" -p "polySurface11";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "polySurface12" -p "loc1:polySurface1";
	setAttr ".t" -type "double3" -234.64718049707426 0 45.124457787898763 ;
createNode mesh -n "polySurfaceShape12" -p "polySurface12";
	setAttr -k off ".v";
	setAttr -s 2 ".iog[0].og";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
createNode transform -n "transform1" -p "loc1:polySurface1";
	setAttr ".v" no;
createNode mesh -n "loc1:polySurfaceShape1" -p "transform1";
	setAttr -k off ".v";
	setAttr ".io" yes;
	setAttr -s 31 ".iog[0].og";
	setAttr ".iog[0].og[0].gcl" -type "componentList" 1 "f[0:1]";
	setAttr ".iog[0].og[1].gcl" -type "componentList" 1 "f[2:3]";
	setAttr ".iog[0].og[2].gcl" -type "componentList" 1 "f[4:5]";
	setAttr ".iog[0].og[3].gcl" -type "componentList" 1 "f[6:7]";
	setAttr ".iog[0].og[4].gcl" -type "componentList" 1 "f[8:13]";
	setAttr ".iog[0].og[5].gcl" -type "componentList" 1 "f[14:19]";
	setAttr ".iog[0].og[6].gcl" -type "componentList" 1 "f[14:19]";
	setAttr ".iog[0].og[7].gcl" -type "componentList" 1 "f[20:45]";
	setAttr ".iog[0].og[8].gcl" -type "componentList" 1 "f[20:45]";
	setAttr ".iog[0].og[9].gcl" -type "componentList" 1 "f[20:25]";
	setAttr ".iog[0].og[10].gcl" -type "componentList" 1 "f[26:31]";
	setAttr ".iog[0].og[11].gcl" -type "componentList" 1 "f[32:37]";
	setAttr ".iog[0].og[12].gcl" -type "componentList" 1 "f[38:43]";
	setAttr ".iog[0].og[13].gcl" -type "componentList" 1 "f[44:45]";
	setAttr ".iog[0].og[14].gcl" -type "componentList" 1 "f[46:47]";
	setAttr ".iog[0].og[15].gcl" -type "componentList" 1 "f[46:47]";
	setAttr ".iog[0].og[16].gcl" -type "componentList" 1 "f[46:47]";
	setAttr ".iog[0].og[17].gcl" -type "componentList" 1 "f[46:47]";
	setAttr ".iog[0].og[18].gcl" -type "componentList" 1 "vtx[0:71]";
	setAttr ".iog[0].og[19].gcl" -type "componentList" 1 "f[0:1]";
	setAttr ".iog[0].og[20].gcl" -type "componentList" 1 "f[2:3]";
	setAttr ".iog[0].og[21].gcl" -type "componentList" 1 "f[4:5]";
	setAttr ".iog[0].og[22].gcl" -type "componentList" 1 "f[6:7]";
	setAttr ".iog[0].og[23].gcl" -type "componentList" 1 "f[8:13]";
	setAttr ".iog[0].og[24].gcl" -type "componentList" 1 "f[14:19]";
	setAttr ".iog[0].og[25].gcl" -type "componentList" 1 "f[20:25]";
	setAttr ".iog[0].og[26].gcl" -type "componentList" 1 "f[26:31]";
	setAttr ".iog[0].og[27].gcl" -type "componentList" 1 "f[32:37]";
	setAttr ".iog[0].og[28].gcl" -type "componentList" 1 "f[38:43]";
	setAttr ".iog[0].og[29].gcl" -type "componentList" 1 "f[44:45]";
	setAttr ".iog[0].og[30].gcl" -type "componentList" 1 "f[46:47]";
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr -s 72 ".uvst[0].uvsp[0:71]" -type "float2" -2.4878459 2.055512905
		 -2.4878459 -1.54434502 2.97667098 2.055512905 2.97667098 -1.54434502 0 0 1 0 0 1.00050604343
		 1 0.99856597 -0.705814 -0.49765599 1.56956196 -0.49765599 -0.705814 1.49765599 1.56956196
		 1.49765599 -1.027467966 -0.99147397 2.027467966 -0.99147397 -1.027467966 1.95644104
		 2.027467966 1.95644104 0.111832 -0.39277801 1.096992016 -0.39277801 1.60579097 0.14341301
		 1.60425103 0.77686697 0.89655799 1.041152 0.015353 1.024492979 -0.60579097 0.39778799
		 -0.59714001 -0.112778 0.111832 -0.39277801 1.096992016 -0.39277801 1.60579097 0.14341301
		 1.60425103 0.77686697 0.89655799 1.041152 0.015353 1.024492979 -0.60579097 0.39778799
		 -0.59714001 -0.112778 0.111832 -0.39277801 1.096992016 -0.39277801 1.60579097 0.14341301
		 1.60425103 0.77686697 0.89655799 1.041152 0.015353 1.024492979 -0.60579097 0.39778799
		 -0.59714001 -0.112778 0.111832 -0.39277801 1.096992016 -0.39277801 1.60579097 0.14341301
		 1.60425103 0.77686697 0.89655799 1.041152 0.015353 1.024492979 -0.60579097 0.39778799
		 -0.59714001 -0.112778 0.111832 -0.39277801 1.096992016 -0.39277801 1.60579097 0.14341301
		 1.60425103 0.77686697 0.89655799 1.041152 0.015353 1.024492979 -0.60579097 0.39778799
		 -0.59714001 -0.112778 0.005566 -1.31408405 1.26042497 -1.31408405 1.90851295 0.036300998
		 1.90655196 1.63164103 1.0051189661 2.29723501 -0.117325 2.25528097 -0.90851301 0.67694002
		 -0.89749497 -0.60891002 0.111832 -0.39277801 1.096992016 -0.39277801 0.89655799 1.041152
		 0.015353 1.024492979 0.111832 -0.39277801 1.096992016 -0.39277801 0.89655799 1.041152
		 0.015353 1.024492979;
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr -s 72 ".vt[0:71]"  -620.94104004 -0.55290598 363.46969604 626.34173584 -0.55290598 363.46969604
		 -620.94104004 -0.55290598 -411.18414307 626.34173584 -0.55290598 -411.18414307 1.081980944 0 -412.65753174
		 113.59137726 0 -412.65753174 1.081980944 0 -524.61651611 113.59137726 0 -524.61651611
		 -1138.40222168 0 354.67025757 -626.60314941 0 354.67025757 -1138.40222168 0 -237.86221313
		 -626.60314941 0 -237.86221313 -1781.56018066 0 166.75288391 -1142.17456055 0 166.75288391
		 -1781.56018066 0 -450.23355103 -1142.17456055 0 -450.23355103 148.49284363 0 368.95114136
		 -237.60475159 0 373.78305054 -434.37985229 0 586.41906738 -430.66943359 0 834.67077637
		 -152.018753052 0 934.77636719 193.25541687 0 923.92565918 433.61627197 0 675.26507568
		 427.72186279 0 475.20968628 -297.33972168 1.19845998 907.82086182 -683.43731689 1.19845998 912.652771
		 -880.21246338 1.19845998 1125.28881836 -876.50201416 1.19845998 1373.54052734 -597.85131836 1.19845998 1473.64611816
		 -252.5771637 1.19845998 1462.79541016 -12.21631813 1.19845998 1214.13476563 -18.11072731 1.19845998 1014.079406738
		 -1644.57470703 18.9748764 196.99917603 -2030.67224121 18.9748764 201.83108521 -2227.44726563 18.9748764 414.46713257
		 -2223.73706055 18.9748764 662.71887207 -1945.086303711 18.9748764 762.82446289 -1599.81213379 18.9748764 751.97375488
		 -1359.45129395 18.9748764 503.31314087 -1365.34570313 18.9748764 303.25772095 -871.77801514 13.38280773 424.26272583
		 -1257.87561035 13.38280773 429.09463501 -1454.65075684 13.38280773 641.73065186 -1450.94030762 13.38280773 889.98236084
		 -1172.28955078 13.38280773 990.08795166 -827.015441895 13.38280773 979.23724365 -586.65460205 13.38280773 730.57666016
		 -592.54901123 13.38280773 530.52124023 -1612.47753906 1.79923606 870.50195313 -1998.57507324 1.79923606 875.3338623
		 -2195.35009766 1.79923606 1087.9699707 -2191.63989258 1.79923606 1336.22167969 -1912.98913574 1.79923606 1436.32727051
		 -1567.71496582 1.79923606 1425.4765625 -1327.35412598 1.79923606 1176.81591797 -1333.24853516 1.79923606 976.76049805
		 -660.88452148 50.33041 -1021.61981201 -1046.98217773 50.33041 -1016.78790283 -1243.75720215 50.33041 -804.15185547
		 -1240.046875 50.33041 -555.90014648 -961.39611816 50.33041 -455.79455566 -616.12194824 50.33041 -466.64526367
		 -375.7611084 50.33041 -715.30584717 -381.65551758 50.33041 -915.36126709 -148.004196167 51.94493484 -217.71658325
		 -242.12034607 51.94493484 -212.88467407 -239.95762634 51.94493484 89.49655151 -169.33018494 51.4402504 98.25802612
		 427.63806152 52.23988342 -223.26551819 333.52191162 52.23988342 -218.43360901 335.68463135 52.23988342 83.94762421
		 406.31207275 51.73520279 92.70909882;
	setAttr -s 108 ".ed[0:107]"  2 0 0 0 3 0 3 2 0 0 1 0 1 3 0 4 5 0 5 6 0
		 6 4 0 5 7 0 7 6 0 8 9 0 9 10 0 10 8 0 9 11 0 11 10 0 12 13 0 13 14 0 14 12 0 13 15 0
		 15 14 0 19 20 0 20 18 0 18 19 0 20 17 0 17 18 0 20 16 0 16 17 0 20 21 0 21 16 0 21 22 0
		 22 16 0 22 23 0 23 16 0 27 28 0 28 26 0 26 27 0 28 25 0 25 26 0 28 24 0 24 25 0 28 29 0
		 29 24 0 29 30 0 30 24 0 30 31 0 31 24 0 35 36 0 36 34 0 34 35 0 36 33 0 33 34 0 36 32 0
		 32 33 0 36 37 0 37 32 0 37 38 0 38 32 0 38 39 0 39 32 0 43 44 0 44 42 0 42 43 0 44 41 0
		 41 42 0 44 40 0 40 41 0 44 45 0 45 40 0 45 46 0 46 40 0 46 47 0 47 40 0 51 52 0 52 50 0
		 50 51 0 52 49 0 49 50 0 52 48 0 48 49 0 52 53 0 53 48 0 53 54 0 54 48 0 54 55 0 55 48 0
		 59 60 0 60 58 0 58 59 0 60 57 0 57 58 0 60 56 0 56 57 0 60 61 0 61 56 0 61 62 0 62 56 0
		 62 63 0 63 56 0 65 66 0 66 64 0 64 65 0 66 67 0 67 64 0 69 70 0 70 68 0 68 69 0 70 71 0
		 71 68 0;
	setAttr -s 144 ".n[0:143]" -type "float3"  0 1 0 0 1 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0
		 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0
		 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0
		 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0
		 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1
		 0 0 1 0 0 1 0 0 1 0 0 1 0 0 1 0 0.0034449999 0.99999398 0.0010310001 0.0034449999
		 0.99999398 0.0010310001 0.0034449999 0.99999398 0.0010310001 0.0068899998 0.99997401
		 0.0020620001 0.0034449999 0.99999398 0.0010310001 0 1 0 0.0034449999 0.99999398 0.0010310001
		 0.0034449999 0.99999398 0.0010310001 0.0034449999 0.99999398 0.0010310001 0.0068899998
		 0.99997401 0.0020620001 0.0034449999 0.99999398 0.0010310001;
	setAttr -s 48 ".fc[0:47]" -type "polyFaces" 
		f 3 0 1 2
		mu 0 3 0 1 2
		f 3 -2 3 4
		mu 0 3 2 1 3
		f 3 5 6 7
		mu 0 3 4 5 6
		f 3 -7 8 9
		mu 0 3 6 5 7
		f 3 10 11 12
		mu 0 3 8 9 10
		f 3 -12 13 14
		mu 0 3 10 9 11
		f 3 15 16 17
		mu 0 3 12 13 14
		f 3 -17 18 19
		mu 0 3 14 13 15
		f 3 20 21 22
		mu 0 3 19 20 18
		f 3 -22 23 24
		mu 0 3 18 20 17
		f 3 -24 25 26
		mu 0 3 17 20 16
		f 3 27 28 -26
		mu 0 3 20 21 16
		f 3 29 30 -29
		mu 0 3 21 22 16
		f 3 -31 31 32
		mu 0 3 16 22 23
		f 3 33 34 35
		mu 0 3 27 28 26
		f 3 -35 36 37
		mu 0 3 26 28 25
		f 3 -37 38 39
		mu 0 3 25 28 24
		f 3 40 41 -39
		mu 0 3 28 29 24
		f 3 42 43 -42
		mu 0 3 29 30 24
		f 3 -44 44 45
		mu 0 3 24 30 31
		f 3 46 47 48
		mu 0 3 35 36 34
		f 3 -48 49 50
		mu 0 3 34 36 33
		f 3 -50 51 52
		mu 0 3 33 36 32
		f 3 53 54 -52
		mu 0 3 36 37 32
		f 3 55 56 -55
		mu 0 3 37 38 32
		f 3 -57 57 58
		mu 0 3 32 38 39
		f 3 59 60 61
		mu 0 3 43 44 42
		f 3 -61 62 63
		mu 0 3 42 44 41
		f 3 -63 64 65
		mu 0 3 41 44 40
		f 3 66 67 -65
		mu 0 3 44 45 40
		f 3 68 69 -68
		mu 0 3 45 46 40
		f 3 -70 70 71
		mu 0 3 40 46 47
		f 3 72 73 74
		mu 0 3 51 52 50
		f 3 -74 75 76
		mu 0 3 50 52 49
		f 3 -76 77 78
		mu 0 3 49 52 48
		f 3 79 80 -78
		mu 0 3 52 53 48
		f 3 81 82 -81
		mu 0 3 53 54 48
		f 3 -83 83 84
		mu 0 3 48 54 55
		f 3 85 86 87
		mu 0 3 59 60 58
		f 3 -87 88 89
		mu 0 3 58 60 57
		f 3 -89 90 91
		mu 0 3 57 60 56
		f 3 92 93 -91
		mu 0 3 60 61 56
		f 3 94 95 -94
		mu 0 3 61 62 56
		f 3 -96 96 97
		mu 0 3 56 62 63
		f 3 98 99 100
		mu 0 3 65 66 64
		f 3 101 102 -100
		mu 0 3 66 67 64
		f 3 103 104 105
		mu 0 3 69 70 68
		f 3 106 107 -105
		mu 0 3 70 71 68;
	setAttr ".cd" -type "dataPolyComponent" Index_Data Edge 0 ;
	setAttr ".cvd" -type "dataPolyComponent" Index_Data Vertex 0 ;
	setAttr ".hfd" -type "dataPolyComponent" Index_Data Face 0 ;
createNode transform -n "pSphere1";
	setAttr ".t" -type "double3" 2100.9498708693209 0 9.4157556942572 ;
	setAttr ".s" -type "double3" 2.1136543913258139 2.113654391325815e-12 2.1136543913258139 ;
createNode mesh -n "pSphereShape1" -p "pSphere1";
	setAttr -k off ".v";
	setAttr ".vir" yes;
	setAttr ".vif" yes;
	setAttr ".uvst[0].uvsn" -type "string" "map1";
	setAttr ".cuvs" -type "string" "map1";
	setAttr ".dcc" -type "string" "Ambient+Diffuse";
	setAttr ".covm[0]"  0 1 1;
	setAttr ".cdvm[0]"  0 1 1;
	setAttr -s 113 ".pt";
	setAttr ".pt[0:1]" -type "float3" -59.071465 0 -13.631877  36.351669 0 
		27.263752 ;
	setAttr ".pt[17:30]" -type "float3" 72.703339 0 18.175835  54.527504 0 0  
		22.719793 0 -49.983551  29.768534 0 -4.5439587  59.071461 0 -13.631876  13.631876 
		0 -18.175835  1.1368684e-13 0.0091325762 -25.596165  1.0757373e-13 0.0091325762 -24.632854  
		-18.175835 0.062919438 -43.771973  -18.175835 0 -49.983551  -36.351669 0.053786863 
		-36.351669  -36.559029 -11.114863 -20.96871  -27.6467 -11.114863 2.4991361e-17  -9.2952766 
		-11.114863 16.424751 ;
	setAttr ".pt[33:50]" -type "float3" -13.631876 0 6.5016918  -13.631876 0 
		30.293627  -13.631876 0 6.5016918  27.263752 0 40.89563  72.703346 0.0029897618 27.263775  
		43.400246 0.0029897618 -22.719772  56.062111 0.0029897618 -22.719772  -3.9424744 
		0 -9.0879173  22.719793 0 -9.0879173  4.5439587 0 -13.631876  1.1368684e-13 0.0091325762 
		13.69816  1.0757373e-13 0.0091325762 -2.4275341  -9.0879173 0.0091325762 -13.565592  
		0 0 -27.263752  -18.175835 0 -18.175835  13.2206 -3.0127394 -15.387691  -3.9721451 
		-3.0127394 2.4991361e-17  13.2206 -3.0127394 15.387691 ;
	setAttr ".pt[53:55]" -type "float3" -13.631876 0 -22.689224  -13.631876 
		0 -0.39953613  -13.631876 0 -22.689301 ;
	setAttr ".pt[57:61]" -type "float3" 27.263752 0 13.631876  -44.838234 0 
		-4.5439587  -32.975861 0 -4.5439587  -20.609495 0 0  0 0 0 ;
	setAttr ".pt[63:65]" -type "float3" 1.1368684e-13 0.0091325762 42.532364  
		1.0757373e-13 0.0091325762 27.890528  1.1368684e-13 0.0091325762 42.532379 ;
	setAttr ".pt[67:70]" -type "float3" 18.175835 0 -13.631876  43.96273 4.4646306 
		-13.971725  28.352068 4.4646306 2.4991361e-17  43.96273 4.4646306 13.971725 ;
	setAttr ".pt[73:75]" -type "float3" -13.631876 0 -62.545189  -13.631876 
		0 -42.306595  -13.631876 0 -62.545219 ;
	setAttr ".pt[77:90]" -type "float3" -9.0879173 0 -13.631876  -66.049217 
		0 -4.5439587  -55.278412 0 -4.5439587  -60.605942 0 -9.0879173  -27.263752 0 22.719793  
		-27.263752 0 36.351669  1.1368684e-13 0.0091325762 78.372345  1.0757373e-13 0.0091325762 
		65.574905  1.1368684e-13 0.0091325762 78.37236  -4.5439587 0.0091325762 79.160355  
		40.89563 0 40.89563  82.17421 11.133139 -12.211732  68.529968 11.133139 2.4991361e-17  
		82.17421 11.133139 12.211732 ;
	setAttr ".pt[93:139]" -type "float3" -13.631876 0 -112.08487  -13.631876 
		0 -94.39566  -13.631876 0 -112.08487  -31.807711 0.053786863 -63.615425  -63.615425 
		0 -36.351669  -92.413773 0 -4.5439587  -82.999748 0 -4.5439587  -111.04632 0.019860109 
		42.375389  -91.084389 0.019860109 94.92337  -50.904892 0.019860109 127.53775  -11.727182 
		0.019860109 154.3123  -14.465897 0.019860109 177.11136  43.322701 0.019860109 172.48814  
		82.500313 0.019860109 145.71359  106.45 0 112.38421  128.33067 0 62.38192  135.87016 
		0 6.9538975  138.35268 0 -51.404613  115.01279 0 -103.9526  78.660072 0 -145.65489  
		20.602882 0.019860109 -154.25365  -22.825853 0.019860109 -163.47945  -66.254684 0.019860109 
		-154.25362  -105.43236 0.019860109 -127.47903  -91.084343 0.019860109 -126.67238  
		-111.04623 0.019860109 -74.12439  -117.92477 0.019860109 -15.874538  -196.75699 4.6738807e+11 
		66.170288  -167.16245 -1.6989872e+10 124.43708  -123.23331 9.158441e+11 163.11214  
		-60.878391 1.8637693e+12 185.86276  0.63724297 1.8637693e+12 195.60582  62.15287 
		1.8637693e+12 185.86273  117.64692 1.8637693e+12 157.58702  142.45929 1.8637693e+12 
		113.74811  165.46741 1.8637693e+12 57.238564  174.32924 1.8637693e+12 1.2872602  
		165.46741 1.8637693e+12 -54.664024  139.74939 1.8637693e+12 -105.13841  110.50196 
		6.693491e+12 -147.88968  48.336182 6.872991e+12 -171.24496  -4.5042038 7.7272433e+12 
		-183.8755  -63.674438 8.1774919e+12 -178.6022  -116.37234 1.8637693e+12 -164.5128  
		-160.41269 1.8637693e+12 -120.47256  -188.68826 1.8637693e+12 -64.978523  -198.43144 
		1.8637693e+12 -3.462914 ;
createNode lightLinker -s -n "lightLinker1";
	setAttr -s 14 ".lnk";
	setAttr -s 14 ".slnk";
createNode displayLayerManager -n "layerManager";
createNode displayLayer -n "defaultLayer";
createNode renderLayerManager -n "renderLayerManager";
createNode renderLayer -n "defaultRenderLayer";
	setAttr ".g" yes;
createNode objectSet -n "loc1:pCube1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId1";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pPlane1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId2";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pPlane2";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId3";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pPlane3";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId4";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:polySurface2";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId5";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__polySurface1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId6";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId7";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__pasted__polySurface1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId8";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__group";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId9";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId10";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group2";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId11";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group3";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId12";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group4";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId13";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group5";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId14";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__pasted__pasted__polySurface1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId15";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__pasted__group";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId16";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:pasted__group5";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId17";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:group6";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId18";
	setAttr ".ihi" 0;
createNode objectSet -n "loc1:default1";
	setAttr ".ihi" 0;
createNode groupId -n "loc1:groupId19";
	setAttr ".ihi" 0;
createNode shadingEngine -n "loc1:blinn1SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo1";
createNode groupId -n "loc1:groupId20";
	setAttr ".ihi" 0;
createNode phong -n "loc1:blinn1SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:blinn1SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo5.png";
createNode place2dTexture -n "loc1:blinn1SG1P2D";
createNode shadingEngine -n "loc1:lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo2";
createNode groupId -n "loc1:groupId21";
	setAttr ".ihi" 0;
createNode phong -n "loc1:lambert4SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:lambert4SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//vine-arch.png";
createNode place2dTexture -n "loc1:lambert4SG1P2D";
createNode shadingEngine -n "loc1:lambert2SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo3";
createNode groupId -n "loc1:groupId22";
	setAttr ".ihi" 0;
createNode phong -n "loc1:lambert2SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:lambert2SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//ground1.png";
createNode place2dTexture -n "loc1:lambert2SG1P2D";
createNode shadingEngine -n "loc1:lambert3SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo4";
createNode groupId -n "loc1:groupId23";
	setAttr ".ihi" 0;
createNode phong -n "loc1:lambert3SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:lambert3SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//ground2.png";
createNode place2dTexture -n "loc1:lambert3SG1P2D";
createNode shadingEngine -n "loc1:lambert4SG2";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo5";
createNode groupId -n "loc1:groupId24";
	setAttr ".ihi" 0;
createNode phong -n "loc1:lambert4SG3";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:lambert4SG3F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//dress6.png";
createNode place2dTexture -n "loc1:lambert4SG3P2D";
createNode shadingEngine -n "loc1:pasted__lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo6";
createNode groupId -n "loc1:groupId25";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__lambert4SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__lambert4SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo5.png";
createNode place2dTexture -n "loc1:pasted__lambert4SG1P2D";
createNode shadingEngine -n "loc1:pasted__pasted__lambert4SG";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo7";
createNode groupId -n "loc1:groupId26";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__lambert4SG1";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__lambert4SG1F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo4.png";
createNode place2dTexture -n "loc1:pasted__pasted__lambert4SG1P2D";
createNode shadingEngine -n "loc1:pasted__pasted__lambert4SG2";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo8";
createNode groupId -n "loc1:groupId27";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__lambert4SG3";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__lambert4SG3F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo3.png";
createNode place2dTexture -n "loc1:pasted__pasted__lambert4SG3P2D";
createNode shadingEngine -n "loc1:pasted__pasted__lambert4SG4";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo9";
createNode groupId -n "loc1:groupId28";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__lambert4SG5";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__lambert4SG5F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo6.png";
createNode place2dTexture -n "loc1:pasted__pasted__lambert4SG5P2D";
createNode shadingEngine -n "loc1:pasted__pasted__lambert4SG6";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo10";
createNode groupId -n "loc1:groupId29";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__lambert4SG7";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__lambert4SG7F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//photo1.png";
createNode place2dTexture -n "loc1:pasted__pasted__lambert4SG7P2D";
createNode shadingEngine -n "loc1:pasted__pasted__lambert4SG8";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo11";
createNode groupId -n "loc1:groupId30";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__lambert4SG9";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__lambert4SG9F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//stone1.png";
createNode place2dTexture -n "loc1:pasted__pasted__lambert4SG9P2D";
createNode shadingEngine -n "loc1:pasted__pasted__pasted__lambert4SG4";
	setAttr ".ihi" 0;
	setAttr -s 2 ".dsm";
	setAttr ".ro" yes;
	setAttr -s 2 ".gn";
createNode materialInfo -n "loc1:materialInfo12";
createNode groupId -n "loc1:groupId31";
	setAttr ".ihi" 0;
createNode phong -n "loc1:pasted__pasted__pasted__lambert4SG5";
	setAttr ".sc" -type "float3" 0.30000001 0.30000001 0.30000001 ;
	setAttr ".cp" 3;
createNode file -n "loc1:pasted__pasted__pasted__lambert4SG5F";
	setAttr ".ftn" -type "string" "/Users/jay/projects/amjulib/Test/ve2/Assets/locations/location1//stone1.png";
createNode place2dTexture -n "loc1:pasted__pasted__pasted__lambert4SG5P2D";
createNode polySeparate -n "polySeparate1";
	setAttr ".ic" 12;
	setAttr -s 12 ".out";
createNode groupId -n "groupId1";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts1";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode groupId -n "groupId2";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts2";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode groupId -n "groupId3";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts3";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode groupId -n "groupId4";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts4";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode groupId -n "groupId5";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts5";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId6";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts6";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId7";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts7";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId8";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts8";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId9";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts9";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId10";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts10";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:5]";
createNode groupId -n "groupId11";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts11";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode groupId -n "groupId12";
	setAttr ".ihi" 0;
createNode groupParts -n "groupParts12";
	setAttr ".ihi" 0;
	setAttr ".ic" -type "componentList" 1 "f[0:1]";
createNode script -n "uiConfigurationScriptNode";
	setAttr ".b" -type "string" (
		"// Maya Mel UI Configuration File.\n//\n//  This script is machine generated.  Edit at your own risk.\n//\n//\n\nglobal string $gMainPane;\nif (`paneLayout -exists $gMainPane`) {\n\n\tglobal int $gUseScenePanelConfig;\n\tint    $useSceneConfig = $gUseScenePanelConfig;\n\tint    $menusOkayInPanels = `optionVar -q allowMenusInPanels`;\tint    $nVisPanes = `paneLayout -q -nvp $gMainPane`;\n\tint    $nPanes = 0;\n\tstring $editorName;\n\tstring $panelName;\n\tstring $itemFilterName;\n\tstring $panelConfig;\n\n\t//\n\t//  get current state of the UI\n\t//\n\tsceneUIReplacement -update $gMainPane;\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Top View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"top\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n"
		+ "                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n"
		+ "                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n"
		+ "                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Top View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"top\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n"
		+ "            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n"
		+ "            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n"
		+ "            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Side View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"side\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n"
		+ "                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n"
		+ "                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n"
		+ "                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Side View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"side\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n"
		+ "            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n"
		+ "            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Front View\")) `;\n\tif (\"\" == $panelName) {\n"
		+ "\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"front\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"wireframe\" \n                -activeOnly 0\n                -ignorePanZoom 0\n                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 0\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n"
		+ "                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n"
		+ "                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Front View\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"front\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"wireframe\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 0\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n"
		+ "            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n"
		+ "            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"modelPanel\" (localizedPanelLabel(\"Persp View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `modelPanel -unParent -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            modelEditor -e \n                -camera \"persp\" \n                -useInteractiveMode 0\n                -displayLights \"default\" \n                -displayAppearance \"smoothShaded\" \n                -activeOnly 0\n                -ignorePanZoom 0\n"
		+ "                -wireframeOnShaded 0\n                -headsUpDisplay 1\n                -selectionHiliteDisplay 1\n                -useDefaultMaterial 0\n                -bufferMode \"double\" \n                -twoSidedLighting 1\n                -backfaceCulling 0\n                -xray 0\n                -jointXray 0\n                -activeComponentsXray 0\n                -displayTextures 1\n                -smoothWireframe 0\n                -lineWidth 1\n                -textureAnisotropic 0\n                -textureHilight 1\n                -textureSampling 2\n                -textureDisplay \"modulate\" \n                -textureMaxSize 16384\n                -fogging 0\n                -fogSource \"fragment\" \n                -fogMode \"linear\" \n                -fogStart 0\n                -fogEnd 100\n                -fogDensity 0.1\n                -fogColor 0.5 0.5 0.5 1 \n                -maxConstantTransparency 1\n                -rendererName \"base_OpenGL_Renderer\" \n                -colorResolution 256 256 \n                -bumpResolution 512 512 \n"
		+ "                -textureCompression 0\n                -transparencyAlgorithm \"frontAndBackCull\" \n                -transpInShadows 0\n                -cullingOverride \"none\" \n                -lowQualityLighting 0\n                -maximumNumHardwareLights 1\n                -occlusionCulling 0\n                -shadingModel 0\n                -useBaseRenderer 0\n                -useReducedRenderer 0\n                -smallObjectCulling 0\n                -smallObjectThreshold -1 \n                -interactiveDisableShadows 0\n                -interactiveBackFaceCull 0\n                -sortTransparent 1\n                -nurbsCurves 1\n                -nurbsSurfaces 1\n                -polymeshes 1\n                -subdivSurfaces 1\n                -planes 1\n                -lights 1\n                -cameras 1\n                -controlVertices 1\n                -hulls 1\n                -grid 1\n                -joints 1\n                -ikHandles 1\n                -deformers 1\n                -dynamics 1\n                -fluids 1\n"
		+ "                -hairSystems 1\n                -follicles 1\n                -nCloths 1\n                -nParticles 1\n                -nRigids 1\n                -dynamicConstraints 1\n                -locators 1\n                -manipulators 1\n                -dimensions 1\n                -handles 1\n                -pivots 1\n                -textures 1\n                -strokes 1\n                -motionTrails 1\n                -shadows 0\n                $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tmodelPanel -edit -l (localizedPanelLabel(\"Persp View\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        modelEditor -e \n            -camera \"persp\" \n            -useInteractiveMode 0\n            -displayLights \"default\" \n            -displayAppearance \"smoothShaded\" \n            -activeOnly 0\n            -ignorePanZoom 0\n            -wireframeOnShaded 0\n            -headsUpDisplay 1\n            -selectionHiliteDisplay 1\n            -useDefaultMaterial 0\n"
		+ "            -bufferMode \"double\" \n            -twoSidedLighting 1\n            -backfaceCulling 0\n            -xray 0\n            -jointXray 0\n            -activeComponentsXray 0\n            -displayTextures 1\n            -smoothWireframe 0\n            -lineWidth 1\n            -textureAnisotropic 0\n            -textureHilight 1\n            -textureSampling 2\n            -textureDisplay \"modulate\" \n            -textureMaxSize 16384\n            -fogging 0\n            -fogSource \"fragment\" \n            -fogMode \"linear\" \n            -fogStart 0\n            -fogEnd 100\n            -fogDensity 0.1\n            -fogColor 0.5 0.5 0.5 1 \n            -maxConstantTransparency 1\n            -rendererName \"base_OpenGL_Renderer\" \n            -colorResolution 256 256 \n            -bumpResolution 512 512 \n            -textureCompression 0\n            -transparencyAlgorithm \"frontAndBackCull\" \n            -transpInShadows 0\n            -cullingOverride \"none\" \n            -lowQualityLighting 0\n            -maximumNumHardwareLights 1\n"
		+ "            -occlusionCulling 0\n            -shadingModel 0\n            -useBaseRenderer 0\n            -useReducedRenderer 0\n            -smallObjectCulling 0\n            -smallObjectThreshold -1 \n            -interactiveDisableShadows 0\n            -interactiveBackFaceCull 0\n            -sortTransparent 1\n            -nurbsCurves 1\n            -nurbsSurfaces 1\n            -polymeshes 1\n            -subdivSurfaces 1\n            -planes 1\n            -lights 1\n            -cameras 1\n            -controlVertices 1\n            -hulls 1\n            -grid 1\n            -joints 1\n            -ikHandles 1\n            -deformers 1\n            -dynamics 1\n            -fluids 1\n            -hairSystems 1\n            -follicles 1\n            -nCloths 1\n            -nParticles 1\n            -nRigids 1\n            -dynamicConstraints 1\n            -locators 1\n            -manipulators 1\n            -dimensions 1\n            -handles 1\n            -pivots 1\n            -textures 1\n            -strokes 1\n            -motionTrails 1\n"
		+ "            -shadows 0\n            $editorName;\nmodelEditor -e -viewSelected 0 $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"outlinerPanel\" (localizedPanelLabel(\"Outliner\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `outlinerPanel -unParent -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels `;\n\t\t\t$editorName = $panelName;\n            outlinerEditor -e \n                -showShapes 0\n                -showAttributes 0\n                -showConnected 0\n                -showAnimCurvesOnly 0\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 1\n                -showAssets 1\n                -showContainedOnly 1\n                -showPublishedAsConnected 0\n                -showContainerContents 1\n                -ignoreDagHierarchy 0\n                -expandConnections 0\n"
		+ "                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 0\n                -highlightActive 1\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"defaultSetFilter\" \n                -showSetMembers 1\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n"
		+ "                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\toutlinerPanel -edit -l (localizedPanelLabel(\"Outliner\")) -mbv $menusOkayInPanels  $panelName;\n\t\t$editorName = $panelName;\n        outlinerEditor -e \n            -showShapes 0\n            -showAttributes 0\n            -showConnected 0\n            -showAnimCurvesOnly 0\n            -showMuteInfo 0\n            -organizeByLayer 1\n            -showAnimLayerWeight 1\n            -autoExpandLayers 1\n            -autoExpand 0\n            -showDagOnly 1\n            -showAssets 1\n            -showContainedOnly 1\n            -showPublishedAsConnected 0\n            -showContainerContents 1\n            -ignoreDagHierarchy 0\n            -expandConnections 0\n            -showUpstreamCurves 1\n            -showUnitlessCurves 1\n            -showCompounds 1\n            -showLeafs 1\n            -showNumericAttrsOnly 0\n            -highlightActive 1\n"
		+ "            -autoSelectNewObjects 0\n            -doNotSelectNewObjects 0\n            -dropIsParent 1\n            -transmitFilters 0\n            -setFilter \"defaultSetFilter\" \n            -showSetMembers 1\n            -allowMultiSelection 1\n            -alwaysToggleSelect 0\n            -directSelect 0\n            -displayMode \"DAG\" \n            -expandObjects 0\n            -setsIgnoreFilters 1\n            -containersIgnoreFilters 0\n            -editAttrName 0\n            -showAttrValues 0\n            -highlightSecondary 0\n            -showUVAttrsOnly 0\n            -showTextureNodesOnly 0\n            -attrAlphaOrder \"default\" \n            -animLayerFilterOptions \"allAffecting\" \n            -sortOrder \"none\" \n            -longNames 0\n            -niceNames 1\n            -showNamespace 1\n            -showPinIcons 0\n            -mapMotionTrails 0\n            $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"graphEditor\" (localizedPanelLabel(\"Graph Editor\")) `;\n"
		+ "\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"graphEditor\" -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n"
		+ "                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n"
		+ "                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Graph Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n"
		+ "            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 1\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 1\n                -showCompounds 0\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 1\n                -doNotSelectNewObjects 0\n                -dropIsParent 1\n                -transmitFilters 1\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 1\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"GraphEd\");\n            animCurveEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 1\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -showResults \"off\" \n                -showBufferCurves \"off\" \n                -smoothness \"fine\" \n                -resultSamples 1\n                -resultScreenSamples 0\n                -resultUpdate \"delayed\" \n                -showUpstreamCurves 1\n                -stackedCurves 0\n                -stackedCurvesMin -1\n                -stackedCurvesMax 1\n                -stackedCurvesSpace 0.2\n                -displayNormalized 0\n                -preSelectionHighlight 0\n                -constrainDrag 0\n                -classicMode 1\n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dopeSheetPanel\" (localizedPanelLabel(\"Dope Sheet\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dopeSheetPanel\" -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n"
		+ "                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n"
		+ "                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n"
		+ "                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dope Sheet\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"OutlineEd\");\n            outlinerEditor -e \n                -showShapes 1\n                -showAttributes 1\n                -showConnected 1\n                -showAnimCurvesOnly 1\n                -showMuteInfo 0\n                -organizeByLayer 1\n                -showAnimLayerWeight 1\n                -autoExpandLayers 1\n                -autoExpand 0\n                -showDagOnly 0\n                -showAssets 1\n                -showContainedOnly 0\n                -showPublishedAsConnected 0\n                -showContainerContents 0\n                -ignoreDagHierarchy 0\n"
		+ "                -expandConnections 1\n                -showUpstreamCurves 1\n                -showUnitlessCurves 0\n                -showCompounds 1\n                -showLeafs 1\n                -showNumericAttrsOnly 1\n                -highlightActive 0\n                -autoSelectNewObjects 0\n                -doNotSelectNewObjects 1\n                -dropIsParent 1\n                -transmitFilters 0\n                -setFilter \"0\" \n                -showSetMembers 0\n                -allowMultiSelection 1\n                -alwaysToggleSelect 0\n                -directSelect 0\n                -displayMode \"DAG\" \n                -expandObjects 0\n                -setsIgnoreFilters 1\n                -containersIgnoreFilters 0\n                -editAttrName 0\n                -showAttrValues 0\n                -highlightSecondary 0\n                -showUVAttrsOnly 0\n                -showTextureNodesOnly 0\n                -attrAlphaOrder \"default\" \n                -animLayerFilterOptions \"allAffecting\" \n                -sortOrder \"none\" \n"
		+ "                -longNames 0\n                -niceNames 1\n                -showNamespace 1\n                -showPinIcons 0\n                -mapMotionTrails 1\n                $editorName;\n\n\t\t\t$editorName = ($panelName+\"DopeSheetEd\");\n            dopeSheetEditor -e \n                -displayKeys 1\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"integer\" \n                -snapValue \"none\" \n                -outliner \"dopeSheetPanel1OutlineEd\" \n                -showSummary 1\n                -showScene 0\n                -hierarchyBelow 0\n                -showTicks 1\n                -selectionWindow 0 0 0 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"clipEditorPanel\" (localizedPanelLabel(\"Trax Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n"
		+ "\t\t\t$panelName = `scriptedPanel -unParent  -type \"clipEditorPanel\" -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Trax Editor\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = clipEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n"
		+ "                -snapValue \"none\" \n                -manageSequencer 0 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"sequenceEditorPanel\" (localizedPanelLabel(\"Camera Sequencer\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"sequenceEditorPanel\" -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Camera Sequencer\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\t\t$editorName = sequenceEditorNameFromPanel($panelName);\n            clipEditor -e \n                -displayKeys 0\n                -displayTangents 0\n                -displayActiveKeys 0\n                -displayActiveKeyTangents 0\n                -displayInfinities 0\n                -autoFit 0\n                -snapTime \"none\" \n                -snapValue \"none\" \n                -manageSequencer 1 \n                $editorName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperGraphPanel\" (localizedPanelLabel(\"Hypergraph Hierarchy\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperGraphPanel\" -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels `;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n"
		+ "                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypergraph Hierarchy\")) -mbv $menusOkayInPanels  $panelName;\n\n\t\t\t$editorName = ($panelName+\"HyperGraphEd\");\n"
		+ "            hyperGraph -e \n                -graphLayoutStyle \"hierarchicalLayout\" \n                -orientation \"horiz\" \n                -mergeConnections 0\n                -zoom 1\n                -animateTransition 0\n                -showRelationships 1\n                -showShapes 0\n                -showDeformers 0\n                -showExpressions 0\n                -showConstraints 0\n                -showUnderworld 0\n                -showInvisible 0\n                -transitionFrames 1\n                -opaqueContainers 0\n                -freeform 0\n                -imagePosition 0 0 \n                -imageScale 1\n                -imageEnabled 0\n                -graphType \"DAG\" \n                -heatMapDisplay 0\n                -updateSelection 1\n                -updateNodeAdded 1\n                -useDrawOverrideColor 0\n                -limitGraphTraversal -1\n                -range 0 0 \n                -iconSize \"smallIcons\" \n                -showCachedConnections 0\n                $editorName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"hyperShadePanel\" (localizedPanelLabel(\"Hypershade\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"hyperShadePanel\" -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Hypershade\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"visorPanel\" (localizedPanelLabel(\"Visor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"visorPanel\" -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Visor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"createNodePanel\" (localizedPanelLabel(\"Create Node\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"createNodePanel\" -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Create Node\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"polyTexturePlacementPanel\" (localizedPanelLabel(\"UV Texture Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"polyTexturePlacementPanel\" -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"UV Texture Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n"
		+ "\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"renderWindowPanel\" (localizedPanelLabel(\"Render View\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"renderWindowPanel\" -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Render View\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextPanel \"blendShapePanel\" (localizedPanelLabel(\"Blend Shape\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\tblendShapePanel -unParent -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels ;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tblendShapePanel -edit -l (localizedPanelLabel(\"Blend Shape\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n"
		+ "\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynRelEdPanel\" (localizedPanelLabel(\"Dynamic Relationships\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynRelEdPanel\" -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Dynamic Relationships\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"relationshipPanel\" (localizedPanelLabel(\"Relationship Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"relationshipPanel\" -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Relationship Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"referenceEditorPanel\" (localizedPanelLabel(\"Reference Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"referenceEditorPanel\" -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Reference Editor\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"componentEditorPanel\" (localizedPanelLabel(\"Component Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"componentEditorPanel\" -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Component Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"dynPaintScriptedPanelType\" (localizedPanelLabel(\"Paint Effects\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"dynPaintScriptedPanelType\" -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Paint Effects\")) -mbv $menusOkayInPanels  $panelName;\n\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\t$panelName = `sceneUIReplacement -getNextScriptedPanel \"scriptEditorPanel\" (localizedPanelLabel(\"Script Editor\")) `;\n\tif (\"\" == $panelName) {\n\t\tif ($useSceneConfig) {\n\t\t\t$panelName = `scriptedPanel -unParent  -type \"scriptEditorPanel\" -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels `;\n\t\t}\n\t} else {\n\t\t$label = `panel -q -label $panelName`;\n\t\tscriptedPanel -edit -l (localizedPanelLabel(\"Script Editor\")) -mbv $menusOkayInPanels  $panelName;\n"
		+ "\t\tif (!$useSceneConfig) {\n\t\t\tpanel -e -l $label $panelName;\n\t\t}\n\t}\n\n\n\tif ($useSceneConfig) {\n        string $configName = `getPanel -cwl (localizedPanelLabel(\"Current Layout\"))`;\n        if (\"\" != $configName) {\n\t\t\tpanelConfiguration -edit -label (localizedPanelLabel(\"Current Layout\")) \n\t\t\t\t-defaultImage \"\"\n\t\t\t\t-image \"\"\n\t\t\t\t-sc false\n\t\t\t\t-configString \"global string $gMainPane; paneLayout -e -cn \\\"quad\\\" -ps 1 68 82 -ps 2 32 82 -ps 3 50 18 -ps 4 50 18 $gMainPane;\"\n\t\t\t\t-removeAllPanels\n\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Top View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera top` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Top View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera top` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Persp View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Persp View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera persp` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"smoothShaded\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 1\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Side View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera side` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Side View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera side` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t-ap false\n\t\t\t\t\t(localizedPanelLabel(\"Front View\")) \n\t\t\t\t\t\"modelPanel\"\n"
		+ "\t\t\t\t\t\"$panelName = `modelPanel -unParent -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels `;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t\t\"modelPanel -edit -l (localizedPanelLabel(\\\"Front View\\\")) -mbv $menusOkayInPanels  $panelName;\\n$editorName = $panelName;\\nmodelEditor -e \\n    -cam `findStartUpCamera front` \\n    -useInteractiveMode 0\\n    -displayLights \\\"default\\\" \\n    -displayAppearance \\\"wireframe\\\" \\n    -activeOnly 0\\n    -ignorePanZoom 0\\n    -wireframeOnShaded 0\\n    -headsUpDisplay 1\\n    -selectionHiliteDisplay 1\\n    -useDefaultMaterial 0\\n    -bufferMode \\\"double\\\" \\n    -twoSidedLighting 1\\n    -backfaceCulling 0\\n    -xray 0\\n    -jointXray 0\\n    -activeComponentsXray 0\\n    -displayTextures 0\\n    -smoothWireframe 0\\n    -lineWidth 1\\n    -textureAnisotropic 0\\n    -textureHilight 1\\n    -textureSampling 2\\n    -textureDisplay \\\"modulate\\\" \\n    -textureMaxSize 16384\\n    -fogging 0\\n    -fogSource \\\"fragment\\\" \\n    -fogMode \\\"linear\\\" \\n    -fogStart 0\\n    -fogEnd 100\\n    -fogDensity 0.1\\n    -fogColor 0.5 0.5 0.5 1 \\n    -maxConstantTransparency 1\\n    -rendererName \\\"base_OpenGL_Renderer\\\" \\n    -colorResolution 256 256 \\n    -bumpResolution 512 512 \\n    -textureCompression 0\\n    -transparencyAlgorithm \\\"frontAndBackCull\\\" \\n    -transpInShadows 0\\n    -cullingOverride \\\"none\\\" \\n    -lowQualityLighting 0\\n    -maximumNumHardwareLights 1\\n    -occlusionCulling 0\\n    -shadingModel 0\\n    -useBaseRenderer 0\\n    -useReducedRenderer 0\\n    -smallObjectCulling 0\\n    -smallObjectThreshold -1 \\n    -interactiveDisableShadows 0\\n    -interactiveBackFaceCull 0\\n    -sortTransparent 1\\n    -nurbsCurves 1\\n    -nurbsSurfaces 1\\n    -polymeshes 1\\n    -subdivSurfaces 1\\n    -planes 1\\n    -lights 1\\n    -cameras 1\\n    -controlVertices 1\\n    -hulls 1\\n    -grid 1\\n    -joints 1\\n    -ikHandles 1\\n    -deformers 1\\n    -dynamics 1\\n    -fluids 1\\n    -hairSystems 1\\n    -follicles 1\\n    -nCloths 1\\n    -nParticles 1\\n    -nRigids 1\\n    -dynamicConstraints 1\\n    -locators 1\\n    -manipulators 1\\n    -dimensions 1\\n    -handles 1\\n    -pivots 1\\n    -textures 1\\n    -strokes 1\\n    -motionTrails 1\\n    -shadows 0\\n    $editorName;\\nmodelEditor -e -viewSelected 0 $editorName\"\n"
		+ "\t\t\t\t$configName;\n\n            setNamedPanelLayout (localizedPanelLabel(\"Current Layout\"));\n        }\n\n        panelHistory -e -clear mainPanelHistory;\n        setFocus `paneLayout -q -p1 $gMainPane`;\n        sceneUIReplacement -deleteRemaining;\n        sceneUIReplacement -clear;\n\t}\n\n\ngrid -spacing 5 -size 12 -divisions 5 -displayAxes yes -displayGridLines yes -displayDivisionLines yes -displayPerspectiveLabels no -displayOrthographicLabels no -displayAxesBold yes -perspectiveLabelPosition axis -orthographicLabelPosition edge;\nviewManip -drawCompass 0 -compassAngle 0 -frontParameters \"\" -homeParameters \"\" -selectionLockParameters \"\";\n}\n");
	setAttr ".st" 3;
createNode script -n "sceneConfigurationScriptNode";
	setAttr ".b" -type "string" "playbackOptions -min 1 -max 24 -ast 1 -aet 48 ";
	setAttr ".st" 6;
createNode polySphere -n "polySphere1";
	setAttr ".ax" -type "double3" 0 1.0000000000000002 2.2204460492503131e-16 ;
	setAttr ".r" 649.79219214574425;
createNode deleteComponent -n "deleteComponent1";
	setAttr ".dc" -type "componentList" 2 "f[0:179]" "f[360:379]";
createNode deleteComponent -n "deleteComponent2";
	setAttr ".dc" -type "componentList" 1 "f[0:19]";
createNode polyTriangulate -n "polyTriangulate1";
	setAttr ".ics" -type "componentList" 1 "f[*]";
createNode polyTweak -n "polyTweak1";
	setAttr ".uopa" yes;
	setAttr -s 8 ".tk";
	setAttr ".tk[0:2]" -type "float3" 123.82434 0 0  102.47533 0.0006127329 
		-51.237663  59.777279 0 -55.507473 ;
	setAttr ".tk[15:19]" -type "float3" 12.809415 0 76.856499  76.856491 -0.053786863 
		102.47533  89.665909 0 68.316879  111.01493 0 34.15844  115.28474 0 12.809415 ;
createNode polyTweak -n "polyTweak2";
	setAttr ".uopa" yes;
	setAttr -s 19 ".tk";
	setAttr ".tk[3:14]" -type "float3" 4.5439587 0.053786863 -95.423134  -45.439587 
		0 -90.879181  -90.879173 0.053786863 -86.33522  -140.86272 0 -49.983547  -149.95065 
		0.053786863 -49.983559  -131.77481 0 -4.5439587  -118.14293 0 -4.5439587  -149.95065 
		0 63.615425  -186.30234 0 149.95065  -104.51105 -0.053786863 154.49461  -9.0879173 
		0 145.40668  22.719793 0 104.51107 ;
	setAttr ".tk[27]" -type "float3" -4.5439577 0 -4.5439587 ;
	setAttr ".tk[140:141]" -type "float3" 0 0 0  0 0 0 ;
createNode deleteComponent -n "deleteComponent3";
	setAttr ".dc" -type "componentList" 1 "f[280:339]";
createNode deleteComponent -n "deleteComponent4";
	setAttr ".dc" -type "componentList" 2 "f[257:267]" "f[269:279]";
createNode deleteComponent -n "deleteComponent5";
	setAttr ".dc" -type "componentList" 1 "f[257]";
createNode deleteComponent -n "deleteComponent6";
	setAttr ".dc" -type "componentList" 1 "f[251:256]";
createNode deleteComponent -n "deleteComponent7";
	setAttr ".dc" -type "componentList" 1 "f[243:250]";
createNode deleteComponent -n "deleteComponent8";
	setAttr ".dc" -type "componentList" 1 "f[241:242]";
createNode deleteComponent -n "deleteComponent9";
	setAttr ".dc" -type "componentList" 1 "f[240]";
select -ne :time1;
	setAttr ".o" 1;
	setAttr ".unw" 1;
select -ne :renderPartition;
	setAttr -s 14 ".st";
select -ne :initialShadingGroup;
	setAttr ".ro" yes;
select -ne :initialParticleSE;
	setAttr ".ro" yes;
select -ne :defaultShaderList1;
	setAttr -s 14 ".s";
select -ne :defaultTextureList1;
	setAttr -s 12 ".tx";
select -ne :postProcessList1;
	setAttr -s 2 ".p";
select -ne :defaultRenderUtilityList1;
	setAttr -s 12 ".u";
select -ne :defaultRenderingList1;
select -ne :renderGlobalsList1;
select -ne :hardwareRenderGlobals;
	setAttr ".ctrs" 256;
	setAttr ".btrs" 512;
select -ne :defaultHardwareRenderGlobals;
	setAttr ".fn" -type "string" "im";
	setAttr ".res" -type "string" "ntsc_4d 646 485 1.333";
connectAttr "groupParts1.og" "polySurfaceShape1.i";
connectAttr "groupId1.id" "polySurfaceShape1.iog.og[0].gid";
connectAttr "loc1:blinn1SG.mwc" "polySurfaceShape1.iog.og[0].gco";
connectAttr "groupParts2.og" "polySurfaceShape2.i";
connectAttr "groupId2.id" "polySurfaceShape2.iog.og[0].gid";
connectAttr "loc1:lambert4SG.mwc" "polySurfaceShape2.iog.og[0].gco";
connectAttr "groupParts3.og" "polySurfaceShape3.i";
connectAttr "groupId3.id" "polySurfaceShape3.iog.og[0].gid";
connectAttr "loc1:lambert2SG.mwc" "polySurfaceShape3.iog.og[0].gco";
connectAttr "groupParts4.og" "polySurfaceShape4.i";
connectAttr "groupId4.id" "polySurfaceShape4.iog.og[0].gid";
connectAttr "loc1:lambert3SG.mwc" "polySurfaceShape4.iog.og[0].gco";
connectAttr "groupParts5.og" "polySurfaceShape5.i";
connectAttr "groupId5.id" "polySurfaceShape5.iog.og[0].gid";
connectAttr "loc1:lambert4SG2.mwc" "polySurfaceShape5.iog.og[0].gco";
connectAttr "groupParts6.og" "polySurfaceShape6.i";
connectAttr "groupId6.id" "polySurfaceShape6.iog.og[0].gid";
connectAttr "loc1:pasted__lambert4SG.mwc" "polySurfaceShape6.iog.og[0].gco";
connectAttr "groupParts7.og" "polySurfaceShape7.i";
connectAttr "groupId7.id" "polySurfaceShape7.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__lambert4SG.mwc" "polySurfaceShape7.iog.og[0].gco"
		;
connectAttr "groupParts8.og" "polySurfaceShape8.i";
connectAttr "groupId8.id" "polySurfaceShape8.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__lambert4SG2.mwc" "polySurfaceShape8.iog.og[0].gco"
		;
connectAttr "groupParts9.og" "polySurfaceShape9.i";
connectAttr "groupId9.id" "polySurfaceShape9.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__lambert4SG4.mwc" "polySurfaceShape9.iog.og[0].gco"
		;
connectAttr "groupParts10.og" "polySurfaceShape10.i";
connectAttr "groupId10.id" "polySurfaceShape10.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__lambert4SG6.mwc" "polySurfaceShape10.iog.og[0].gco"
		;
connectAttr "groupParts11.og" "polySurfaceShape11.i";
connectAttr "groupId11.id" "polySurfaceShape11.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__lambert4SG8.mwc" "polySurfaceShape11.iog.og[0].gco"
		;
connectAttr "groupParts12.og" "polySurfaceShape12.i";
connectAttr "groupId12.id" "polySurfaceShape12.iog.og[0].gid";
connectAttr "loc1:pasted__pasted__pasted__lambert4SG4.mwc" "polySurfaceShape12.iog.og[0].gco"
		;
connectAttr "loc1:groupId1.id" "loc1:polySurfaceShape1.iog.og[0].gid";
connectAttr "loc1:pCube1.mwc" "loc1:polySurfaceShape1.iog.og[0].gco";
connectAttr "loc1:groupId2.id" "loc1:polySurfaceShape1.iog.og[1].gid";
connectAttr "loc1:pPlane1.mwc" "loc1:polySurfaceShape1.iog.og[1].gco";
connectAttr "loc1:groupId3.id" "loc1:polySurfaceShape1.iog.og[2].gid";
connectAttr "loc1:pPlane2.mwc" "loc1:polySurfaceShape1.iog.og[2].gco";
connectAttr "loc1:groupId4.id" "loc1:polySurfaceShape1.iog.og[3].gid";
connectAttr "loc1:pPlane3.mwc" "loc1:polySurfaceShape1.iog.og[3].gco";
connectAttr "loc1:groupId5.id" "loc1:polySurfaceShape1.iog.og[4].gid";
connectAttr "loc1:polySurface2.mwc" "loc1:polySurfaceShape1.iog.og[4].gco";
connectAttr "loc1:groupId6.id" "loc1:polySurfaceShape1.iog.og[5].gid";
connectAttr "loc1:pasted__polySurface1.mwc" "loc1:polySurfaceShape1.iog.og[5].gco"
		;
connectAttr "loc1:groupId7.id" "loc1:polySurfaceShape1.iog.og[6].gid";
connectAttr "loc1:group.mwc" "loc1:polySurfaceShape1.iog.og[6].gco";
connectAttr "loc1:groupId8.id" "loc1:polySurfaceShape1.iog.og[7].gid";
connectAttr "loc1:pasted__pasted__polySurface1.mwc" "loc1:polySurfaceShape1.iog.og[7].gco"
		;
connectAttr "loc1:groupId9.id" "loc1:polySurfaceShape1.iog.og[8].gid";
connectAttr "loc1:pasted__group.mwc" "loc1:polySurfaceShape1.iog.og[8].gco";
connectAttr "loc1:groupId10.id" "loc1:polySurfaceShape1.iog.og[9].gid";
connectAttr "loc1:group1.mwc" "loc1:polySurfaceShape1.iog.og[9].gco";
connectAttr "loc1:groupId11.id" "loc1:polySurfaceShape1.iog.og[10].gid";
connectAttr "loc1:group2.mwc" "loc1:polySurfaceShape1.iog.og[10].gco";
connectAttr "loc1:groupId12.id" "loc1:polySurfaceShape1.iog.og[11].gid";
connectAttr "loc1:group3.mwc" "loc1:polySurfaceShape1.iog.og[11].gco";
connectAttr "loc1:groupId13.id" "loc1:polySurfaceShape1.iog.og[12].gid";
connectAttr "loc1:group4.mwc" "loc1:polySurfaceShape1.iog.og[12].gco";
connectAttr "loc1:groupId14.id" "loc1:polySurfaceShape1.iog.og[13].gid";
connectAttr "loc1:group5.mwc" "loc1:polySurfaceShape1.iog.og[13].gco";
connectAttr "loc1:groupId15.id" "loc1:polySurfaceShape1.iog.og[14].gid";
connectAttr "loc1:pasted__pasted__pasted__polySurface1.mwc" "loc1:polySurfaceShape1.iog.og[14].gco"
		;
connectAttr "loc1:groupId16.id" "loc1:polySurfaceShape1.iog.og[15].gid";
connectAttr "loc1:pasted__pasted__group.mwc" "loc1:polySurfaceShape1.iog.og[15].gco"
		;
connectAttr "loc1:groupId17.id" "loc1:polySurfaceShape1.iog.og[16].gid";
connectAttr "loc1:pasted__group5.mwc" "loc1:polySurfaceShape1.iog.og[16].gco";
connectAttr "loc1:groupId18.id" "loc1:polySurfaceShape1.iog.og[17].gid";
connectAttr "loc1:group6.mwc" "loc1:polySurfaceShape1.iog.og[17].gco";
connectAttr "loc1:groupId19.id" "loc1:polySurfaceShape1.iog.og[18].gid";
connectAttr "loc1:default1.mwc" "loc1:polySurfaceShape1.iog.og[18].gco";
connectAttr "loc1:groupId20.id" "loc1:polySurfaceShape1.iog.og[19].gid";
connectAttr "loc1:blinn1SG.mwc" "loc1:polySurfaceShape1.iog.og[19].gco";
connectAttr "loc1:groupId21.id" "loc1:polySurfaceShape1.iog.og[20].gid";
connectAttr "loc1:lambert4SG.mwc" "loc1:polySurfaceShape1.iog.og[20].gco";
connectAttr "loc1:groupId22.id" "loc1:polySurfaceShape1.iog.og[21].gid";
connectAttr "loc1:lambert2SG.mwc" "loc1:polySurfaceShape1.iog.og[21].gco";
connectAttr "loc1:groupId23.id" "loc1:polySurfaceShape1.iog.og[22].gid";
connectAttr "loc1:lambert3SG.mwc" "loc1:polySurfaceShape1.iog.og[22].gco";
connectAttr "loc1:groupId24.id" "loc1:polySurfaceShape1.iog.og[23].gid";
connectAttr "loc1:lambert4SG2.mwc" "loc1:polySurfaceShape1.iog.og[23].gco";
connectAttr "loc1:groupId25.id" "loc1:polySurfaceShape1.iog.og[24].gid";
connectAttr "loc1:pasted__lambert4SG.mwc" "loc1:polySurfaceShape1.iog.og[24].gco"
		;
connectAttr "loc1:groupId26.id" "loc1:polySurfaceShape1.iog.og[25].gid";
connectAttr "loc1:pasted__pasted__lambert4SG.mwc" "loc1:polySurfaceShape1.iog.og[25].gco"
		;
connectAttr "loc1:groupId27.id" "loc1:polySurfaceShape1.iog.og[26].gid";
connectAttr "loc1:pasted__pasted__lambert4SG2.mwc" "loc1:polySurfaceShape1.iog.og[26].gco"
		;
connectAttr "loc1:groupId28.id" "loc1:polySurfaceShape1.iog.og[27].gid";
connectAttr "loc1:pasted__pasted__lambert4SG4.mwc" "loc1:polySurfaceShape1.iog.og[27].gco"
		;
connectAttr "loc1:groupId29.id" "loc1:polySurfaceShape1.iog.og[28].gid";
connectAttr "loc1:pasted__pasted__lambert4SG6.mwc" "loc1:polySurfaceShape1.iog.og[28].gco"
		;
connectAttr "loc1:groupId30.id" "loc1:polySurfaceShape1.iog.og[29].gid";
connectAttr "loc1:pasted__pasted__lambert4SG8.mwc" "loc1:polySurfaceShape1.iog.og[29].gco"
		;
connectAttr "loc1:groupId31.id" "loc1:polySurfaceShape1.iog.og[30].gid";
connectAttr "loc1:pasted__pasted__pasted__lambert4SG4.mwc" "loc1:polySurfaceShape1.iog.og[30].gco"
		;
connectAttr "deleteComponent9.og" "pSphereShape1.i";
relationship "link" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:blinn1SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:lambert2SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:lambert3SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:lambert4SG2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__lambert4SG.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__lambert4SG2.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__lambert4SG4.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__lambert4SG6.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__lambert4SG8.message" ":defaultLightSet.message";
relationship "link" ":lightLinker1" "loc1:pasted__pasted__pasted__lambert4SG4.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialShadingGroup.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" ":initialParticleSE.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:blinn1SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:lambert2SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:lambert3SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:lambert4SG2.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__lambert4SG.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__lambert4SG2.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__lambert4SG4.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__lambert4SG6.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__lambert4SG8.message" ":defaultLightSet.message";
relationship "shadowLink" ":lightLinker1" "loc1:pasted__pasted__pasted__lambert4SG4.message" ":defaultLightSet.message";
connectAttr "layerManager.dli[0]" "defaultLayer.id";
connectAttr "renderLayerManager.rlmi[0]" "defaultRenderLayer.rlid";
connectAttr "loc1:groupId1.msg" "loc1:pCube1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[0]" "loc1:pCube1.dsm" -na;
connectAttr "loc1:groupId2.msg" "loc1:pPlane1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[1]" "loc1:pPlane1.dsm" -na;
connectAttr "loc1:groupId3.msg" "loc1:pPlane2.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[2]" "loc1:pPlane2.dsm" -na;
connectAttr "loc1:groupId4.msg" "loc1:pPlane3.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[3]" "loc1:pPlane3.dsm" -na;
connectAttr "loc1:groupId5.msg" "loc1:polySurface2.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[4]" "loc1:polySurface2.dsm" -na;
connectAttr "loc1:groupId6.msg" "loc1:pasted__polySurface1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[5]" "loc1:pasted__polySurface1.dsm" -na
		;
connectAttr "loc1:groupId7.msg" "loc1:group.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[6]" "loc1:group.dsm" -na;
connectAttr "loc1:groupId8.msg" "loc1:pasted__pasted__polySurface1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[7]" "loc1:pasted__pasted__polySurface1.dsm"
		 -na;
connectAttr "loc1:groupId9.msg" "loc1:pasted__group.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[8]" "loc1:pasted__group.dsm" -na;
connectAttr "loc1:groupId10.msg" "loc1:group1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[9]" "loc1:group1.dsm" -na;
connectAttr "loc1:groupId11.msg" "loc1:group2.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[10]" "loc1:group2.dsm" -na;
connectAttr "loc1:groupId12.msg" "loc1:group3.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[11]" "loc1:group3.dsm" -na;
connectAttr "loc1:groupId13.msg" "loc1:group4.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[12]" "loc1:group4.dsm" -na;
connectAttr "loc1:groupId14.msg" "loc1:group5.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[13]" "loc1:group5.dsm" -na;
connectAttr "loc1:groupId15.msg" "loc1:pasted__pasted__pasted__polySurface1.gn" 
		-na;
connectAttr "loc1:polySurfaceShape1.iog.og[14]" "loc1:pasted__pasted__pasted__polySurface1.dsm"
		 -na;
connectAttr "loc1:groupId16.msg" "loc1:pasted__pasted__group.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[15]" "loc1:pasted__pasted__group.dsm"
		 -na;
connectAttr "loc1:groupId17.msg" "loc1:pasted__group5.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[16]" "loc1:pasted__group5.dsm" -na;
connectAttr "loc1:groupId18.msg" "loc1:group6.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[17]" "loc1:group6.dsm" -na;
connectAttr "loc1:groupId19.msg" "loc1:default1.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[18]" "loc1:default1.dsm" -na;
connectAttr "loc1:blinn1SG1.oc" "loc1:blinn1SG.ss";
connectAttr "loc1:groupId20.msg" "loc1:blinn1SG.gn" -na;
connectAttr "groupId1.msg" "loc1:blinn1SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[19]" "loc1:blinn1SG.dsm" -na;
connectAttr "polySurfaceShape1.iog.og[0]" "loc1:blinn1SG.dsm" -na;
connectAttr "loc1:blinn1SG.msg" "loc1:materialInfo1.sg";
connectAttr "loc1:blinn1SG1.msg" "loc1:materialInfo1.m";
connectAttr "loc1:blinn1SG1F.msg" "loc1:materialInfo1.t" -na;
connectAttr "loc1:blinn1SG1F.oc" "loc1:blinn1SG1.c";
connectAttr "loc1:blinn1SG1P2D.c" "loc1:blinn1SG1F.c";
connectAttr "loc1:blinn1SG1P2D.tf" "loc1:blinn1SG1F.tf";
connectAttr "loc1:blinn1SG1P2D.rf" "loc1:blinn1SG1F.rf";
connectAttr "loc1:blinn1SG1P2D.s" "loc1:blinn1SG1F.s";
connectAttr "loc1:blinn1SG1P2D.wu" "loc1:blinn1SG1F.wu";
connectAttr "loc1:blinn1SG1P2D.wv" "loc1:blinn1SG1F.wv";
connectAttr "loc1:blinn1SG1P2D.re" "loc1:blinn1SG1F.re";
connectAttr "loc1:blinn1SG1P2D.of" "loc1:blinn1SG1F.of";
connectAttr "loc1:blinn1SG1P2D.r" "loc1:blinn1SG1F.ro";
connectAttr "loc1:blinn1SG1P2D.o" "loc1:blinn1SG1F.uv";
connectAttr "loc1:blinn1SG1P2D.ofs" "loc1:blinn1SG1F.fs";
connectAttr "loc1:lambert4SG1.oc" "loc1:lambert4SG.ss";
connectAttr "loc1:groupId21.msg" "loc1:lambert4SG.gn" -na;
connectAttr "groupId2.msg" "loc1:lambert4SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[20]" "loc1:lambert4SG.dsm" -na;
connectAttr "polySurfaceShape2.iog.og[0]" "loc1:lambert4SG.dsm" -na;
connectAttr "loc1:lambert4SG.msg" "loc1:materialInfo2.sg";
connectAttr "loc1:lambert4SG1.msg" "loc1:materialInfo2.m";
connectAttr "loc1:lambert4SG1F.msg" "loc1:materialInfo2.t" -na;
connectAttr "loc1:lambert4SG1F.oc" "loc1:lambert4SG1.c";
connectAttr "loc1:lambert4SG1P2D.c" "loc1:lambert4SG1F.c";
connectAttr "loc1:lambert4SG1P2D.tf" "loc1:lambert4SG1F.tf";
connectAttr "loc1:lambert4SG1P2D.rf" "loc1:lambert4SG1F.rf";
connectAttr "loc1:lambert4SG1P2D.s" "loc1:lambert4SG1F.s";
connectAttr "loc1:lambert4SG1P2D.wu" "loc1:lambert4SG1F.wu";
connectAttr "loc1:lambert4SG1P2D.wv" "loc1:lambert4SG1F.wv";
connectAttr "loc1:lambert4SG1P2D.re" "loc1:lambert4SG1F.re";
connectAttr "loc1:lambert4SG1P2D.of" "loc1:lambert4SG1F.of";
connectAttr "loc1:lambert4SG1P2D.r" "loc1:lambert4SG1F.ro";
connectAttr "loc1:lambert4SG1P2D.o" "loc1:lambert4SG1F.uv";
connectAttr "loc1:lambert4SG1P2D.ofs" "loc1:lambert4SG1F.fs";
connectAttr "loc1:lambert2SG1.oc" "loc1:lambert2SG.ss";
connectAttr "loc1:groupId22.msg" "loc1:lambert2SG.gn" -na;
connectAttr "groupId3.msg" "loc1:lambert2SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[21]" "loc1:lambert2SG.dsm" -na;
connectAttr "polySurfaceShape3.iog.og[0]" "loc1:lambert2SG.dsm" -na;
connectAttr "loc1:lambert2SG.msg" "loc1:materialInfo3.sg";
connectAttr "loc1:lambert2SG1.msg" "loc1:materialInfo3.m";
connectAttr "loc1:lambert2SG1F.msg" "loc1:materialInfo3.t" -na;
connectAttr "loc1:lambert2SG1F.oc" "loc1:lambert2SG1.c";
connectAttr "loc1:lambert2SG1P2D.c" "loc1:lambert2SG1F.c";
connectAttr "loc1:lambert2SG1P2D.tf" "loc1:lambert2SG1F.tf";
connectAttr "loc1:lambert2SG1P2D.rf" "loc1:lambert2SG1F.rf";
connectAttr "loc1:lambert2SG1P2D.s" "loc1:lambert2SG1F.s";
connectAttr "loc1:lambert2SG1P2D.wu" "loc1:lambert2SG1F.wu";
connectAttr "loc1:lambert2SG1P2D.wv" "loc1:lambert2SG1F.wv";
connectAttr "loc1:lambert2SG1P2D.re" "loc1:lambert2SG1F.re";
connectAttr "loc1:lambert2SG1P2D.of" "loc1:lambert2SG1F.of";
connectAttr "loc1:lambert2SG1P2D.r" "loc1:lambert2SG1F.ro";
connectAttr "loc1:lambert2SG1P2D.o" "loc1:lambert2SG1F.uv";
connectAttr "loc1:lambert2SG1P2D.ofs" "loc1:lambert2SG1F.fs";
connectAttr "loc1:lambert3SG1.oc" "loc1:lambert3SG.ss";
connectAttr "loc1:groupId23.msg" "loc1:lambert3SG.gn" -na;
connectAttr "groupId4.msg" "loc1:lambert3SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[22]" "loc1:lambert3SG.dsm" -na;
connectAttr "polySurfaceShape4.iog.og[0]" "loc1:lambert3SG.dsm" -na;
connectAttr "loc1:lambert3SG.msg" "loc1:materialInfo4.sg";
connectAttr "loc1:lambert3SG1.msg" "loc1:materialInfo4.m";
connectAttr "loc1:lambert3SG1F.msg" "loc1:materialInfo4.t" -na;
connectAttr "loc1:lambert3SG1F.oc" "loc1:lambert3SG1.c";
connectAttr "loc1:lambert3SG1P2D.c" "loc1:lambert3SG1F.c";
connectAttr "loc1:lambert3SG1P2D.tf" "loc1:lambert3SG1F.tf";
connectAttr "loc1:lambert3SG1P2D.rf" "loc1:lambert3SG1F.rf";
connectAttr "loc1:lambert3SG1P2D.s" "loc1:lambert3SG1F.s";
connectAttr "loc1:lambert3SG1P2D.wu" "loc1:lambert3SG1F.wu";
connectAttr "loc1:lambert3SG1P2D.wv" "loc1:lambert3SG1F.wv";
connectAttr "loc1:lambert3SG1P2D.re" "loc1:lambert3SG1F.re";
connectAttr "loc1:lambert3SG1P2D.of" "loc1:lambert3SG1F.of";
connectAttr "loc1:lambert3SG1P2D.r" "loc1:lambert3SG1F.ro";
connectAttr "loc1:lambert3SG1P2D.o" "loc1:lambert3SG1F.uv";
connectAttr "loc1:lambert3SG1P2D.ofs" "loc1:lambert3SG1F.fs";
connectAttr "loc1:lambert4SG3.oc" "loc1:lambert4SG2.ss";
connectAttr "loc1:groupId24.msg" "loc1:lambert4SG2.gn" -na;
connectAttr "groupId5.msg" "loc1:lambert4SG2.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[23]" "loc1:lambert4SG2.dsm" -na;
connectAttr "polySurfaceShape5.iog.og[0]" "loc1:lambert4SG2.dsm" -na;
connectAttr "loc1:lambert4SG2.msg" "loc1:materialInfo5.sg";
connectAttr "loc1:lambert4SG3.msg" "loc1:materialInfo5.m";
connectAttr "loc1:lambert4SG3F.msg" "loc1:materialInfo5.t" -na;
connectAttr "loc1:lambert4SG3F.oc" "loc1:lambert4SG3.c";
connectAttr "loc1:lambert4SG3P2D.c" "loc1:lambert4SG3F.c";
connectAttr "loc1:lambert4SG3P2D.tf" "loc1:lambert4SG3F.tf";
connectAttr "loc1:lambert4SG3P2D.rf" "loc1:lambert4SG3F.rf";
connectAttr "loc1:lambert4SG3P2D.s" "loc1:lambert4SG3F.s";
connectAttr "loc1:lambert4SG3P2D.wu" "loc1:lambert4SG3F.wu";
connectAttr "loc1:lambert4SG3P2D.wv" "loc1:lambert4SG3F.wv";
connectAttr "loc1:lambert4SG3P2D.re" "loc1:lambert4SG3F.re";
connectAttr "loc1:lambert4SG3P2D.of" "loc1:lambert4SG3F.of";
connectAttr "loc1:lambert4SG3P2D.r" "loc1:lambert4SG3F.ro";
connectAttr "loc1:lambert4SG3P2D.o" "loc1:lambert4SG3F.uv";
connectAttr "loc1:lambert4SG3P2D.ofs" "loc1:lambert4SG3F.fs";
connectAttr "loc1:pasted__lambert4SG1.oc" "loc1:pasted__lambert4SG.ss";
connectAttr "loc1:groupId25.msg" "loc1:pasted__lambert4SG.gn" -na;
connectAttr "groupId6.msg" "loc1:pasted__lambert4SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[24]" "loc1:pasted__lambert4SG.dsm" -na
		;
connectAttr "polySurfaceShape6.iog.og[0]" "loc1:pasted__lambert4SG.dsm" -na;
connectAttr "loc1:pasted__lambert4SG.msg" "loc1:materialInfo6.sg";
connectAttr "loc1:pasted__lambert4SG1.msg" "loc1:materialInfo6.m";
connectAttr "loc1:pasted__lambert4SG1F.msg" "loc1:materialInfo6.t" -na;
connectAttr "loc1:pasted__lambert4SG1F.oc" "loc1:pasted__lambert4SG1.c";
connectAttr "loc1:pasted__lambert4SG1P2D.c" "loc1:pasted__lambert4SG1F.c";
connectAttr "loc1:pasted__lambert4SG1P2D.tf" "loc1:pasted__lambert4SG1F.tf";
connectAttr "loc1:pasted__lambert4SG1P2D.rf" "loc1:pasted__lambert4SG1F.rf";
connectAttr "loc1:pasted__lambert4SG1P2D.s" "loc1:pasted__lambert4SG1F.s";
connectAttr "loc1:pasted__lambert4SG1P2D.wu" "loc1:pasted__lambert4SG1F.wu";
connectAttr "loc1:pasted__lambert4SG1P2D.wv" "loc1:pasted__lambert4SG1F.wv";
connectAttr "loc1:pasted__lambert4SG1P2D.re" "loc1:pasted__lambert4SG1F.re";
connectAttr "loc1:pasted__lambert4SG1P2D.of" "loc1:pasted__lambert4SG1F.of";
connectAttr "loc1:pasted__lambert4SG1P2D.r" "loc1:pasted__lambert4SG1F.ro";
connectAttr "loc1:pasted__lambert4SG1P2D.o" "loc1:pasted__lambert4SG1F.uv";
connectAttr "loc1:pasted__lambert4SG1P2D.ofs" "loc1:pasted__lambert4SG1F.fs";
connectAttr "loc1:pasted__pasted__lambert4SG1.oc" "loc1:pasted__pasted__lambert4SG.ss"
		;
connectAttr "loc1:groupId26.msg" "loc1:pasted__pasted__lambert4SG.gn" -na;
connectAttr "groupId7.msg" "loc1:pasted__pasted__lambert4SG.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[25]" "loc1:pasted__pasted__lambert4SG.dsm"
		 -na;
connectAttr "polySurfaceShape7.iog.og[0]" "loc1:pasted__pasted__lambert4SG.dsm" 
		-na;
connectAttr "loc1:pasted__pasted__lambert4SG.msg" "loc1:materialInfo7.sg";
connectAttr "loc1:pasted__pasted__lambert4SG1.msg" "loc1:materialInfo7.m";
connectAttr "loc1:pasted__pasted__lambert4SG1F.msg" "loc1:materialInfo7.t" -na;
connectAttr "loc1:pasted__pasted__lambert4SG1F.oc" "loc1:pasted__pasted__lambert4SG1.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.c" "loc1:pasted__pasted__lambert4SG1F.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.tf" "loc1:pasted__pasted__lambert4SG1F.tf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.rf" "loc1:pasted__pasted__lambert4SG1F.rf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.s" "loc1:pasted__pasted__lambert4SG1F.s"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.wu" "loc1:pasted__pasted__lambert4SG1F.wu"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.wv" "loc1:pasted__pasted__lambert4SG1F.wv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.re" "loc1:pasted__pasted__lambert4SG1F.re"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.of" "loc1:pasted__pasted__lambert4SG1F.of"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.r" "loc1:pasted__pasted__lambert4SG1F.ro"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.o" "loc1:pasted__pasted__lambert4SG1F.uv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.ofs" "loc1:pasted__pasted__lambert4SG1F.fs"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3.oc" "loc1:pasted__pasted__lambert4SG2.ss"
		;
connectAttr "loc1:groupId27.msg" "loc1:pasted__pasted__lambert4SG2.gn" -na;
connectAttr "groupId8.msg" "loc1:pasted__pasted__lambert4SG2.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[26]" "loc1:pasted__pasted__lambert4SG2.dsm"
		 -na;
connectAttr "polySurfaceShape8.iog.og[0]" "loc1:pasted__pasted__lambert4SG2.dsm"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG2.msg" "loc1:materialInfo8.sg";
connectAttr "loc1:pasted__pasted__lambert4SG3.msg" "loc1:materialInfo8.m";
connectAttr "loc1:pasted__pasted__lambert4SG3F.msg" "loc1:materialInfo8.t" -na;
connectAttr "loc1:pasted__pasted__lambert4SG3F.oc" "loc1:pasted__pasted__lambert4SG3.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.c" "loc1:pasted__pasted__lambert4SG3F.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.tf" "loc1:pasted__pasted__lambert4SG3F.tf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.rf" "loc1:pasted__pasted__lambert4SG3F.rf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.s" "loc1:pasted__pasted__lambert4SG3F.s"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.wu" "loc1:pasted__pasted__lambert4SG3F.wu"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.wv" "loc1:pasted__pasted__lambert4SG3F.wv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.re" "loc1:pasted__pasted__lambert4SG3F.re"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.of" "loc1:pasted__pasted__lambert4SG3F.of"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.r" "loc1:pasted__pasted__lambert4SG3F.ro"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.o" "loc1:pasted__pasted__lambert4SG3F.uv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.ofs" "loc1:pasted__pasted__lambert4SG3F.fs"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5.oc" "loc1:pasted__pasted__lambert4SG4.ss"
		;
connectAttr "loc1:groupId28.msg" "loc1:pasted__pasted__lambert4SG4.gn" -na;
connectAttr "groupId9.msg" "loc1:pasted__pasted__lambert4SG4.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[27]" "loc1:pasted__pasted__lambert4SG4.dsm"
		 -na;
connectAttr "polySurfaceShape9.iog.og[0]" "loc1:pasted__pasted__lambert4SG4.dsm"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG4.msg" "loc1:materialInfo9.sg";
connectAttr "loc1:pasted__pasted__lambert4SG5.msg" "loc1:materialInfo9.m";
connectAttr "loc1:pasted__pasted__lambert4SG5F.msg" "loc1:materialInfo9.t" -na;
connectAttr "loc1:pasted__pasted__lambert4SG5F.oc" "loc1:pasted__pasted__lambert4SG5.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.c" "loc1:pasted__pasted__lambert4SG5F.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.tf" "loc1:pasted__pasted__lambert4SG5F.tf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.rf" "loc1:pasted__pasted__lambert4SG5F.rf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.s" "loc1:pasted__pasted__lambert4SG5F.s"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.wu" "loc1:pasted__pasted__lambert4SG5F.wu"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.wv" "loc1:pasted__pasted__lambert4SG5F.wv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.re" "loc1:pasted__pasted__lambert4SG5F.re"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.of" "loc1:pasted__pasted__lambert4SG5F.of"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.r" "loc1:pasted__pasted__lambert4SG5F.ro"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.o" "loc1:pasted__pasted__lambert4SG5F.uv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.ofs" "loc1:pasted__pasted__lambert4SG5F.fs"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7.oc" "loc1:pasted__pasted__lambert4SG6.ss"
		;
connectAttr "loc1:groupId29.msg" "loc1:pasted__pasted__lambert4SG6.gn" -na;
connectAttr "groupId10.msg" "loc1:pasted__pasted__lambert4SG6.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[28]" "loc1:pasted__pasted__lambert4SG6.dsm"
		 -na;
connectAttr "polySurfaceShape10.iog.og[0]" "loc1:pasted__pasted__lambert4SG6.dsm"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG6.msg" "loc1:materialInfo10.sg";
connectAttr "loc1:pasted__pasted__lambert4SG7.msg" "loc1:materialInfo10.m";
connectAttr "loc1:pasted__pasted__lambert4SG7F.msg" "loc1:materialInfo10.t" -na;
connectAttr "loc1:pasted__pasted__lambert4SG7F.oc" "loc1:pasted__pasted__lambert4SG7.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.c" "loc1:pasted__pasted__lambert4SG7F.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.tf" "loc1:pasted__pasted__lambert4SG7F.tf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.rf" "loc1:pasted__pasted__lambert4SG7F.rf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.s" "loc1:pasted__pasted__lambert4SG7F.s"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.wu" "loc1:pasted__pasted__lambert4SG7F.wu"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.wv" "loc1:pasted__pasted__lambert4SG7F.wv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.re" "loc1:pasted__pasted__lambert4SG7F.re"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.of" "loc1:pasted__pasted__lambert4SG7F.of"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.r" "loc1:pasted__pasted__lambert4SG7F.ro"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.o" "loc1:pasted__pasted__lambert4SG7F.uv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.ofs" "loc1:pasted__pasted__lambert4SG7F.fs"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9.oc" "loc1:pasted__pasted__lambert4SG8.ss"
		;
connectAttr "loc1:groupId30.msg" "loc1:pasted__pasted__lambert4SG8.gn" -na;
connectAttr "groupId11.msg" "loc1:pasted__pasted__lambert4SG8.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[29]" "loc1:pasted__pasted__lambert4SG8.dsm"
		 -na;
connectAttr "polySurfaceShape11.iog.og[0]" "loc1:pasted__pasted__lambert4SG8.dsm"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG8.msg" "loc1:materialInfo11.sg";
connectAttr "loc1:pasted__pasted__lambert4SG9.msg" "loc1:materialInfo11.m";
connectAttr "loc1:pasted__pasted__lambert4SG9F.msg" "loc1:materialInfo11.t" -na;
connectAttr "loc1:pasted__pasted__lambert4SG9F.oc" "loc1:pasted__pasted__lambert4SG9.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.c" "loc1:pasted__pasted__lambert4SG9F.c"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.tf" "loc1:pasted__pasted__lambert4SG9F.tf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.rf" "loc1:pasted__pasted__lambert4SG9F.rf"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.s" "loc1:pasted__pasted__lambert4SG9F.s"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.wu" "loc1:pasted__pasted__lambert4SG9F.wu"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.wv" "loc1:pasted__pasted__lambert4SG9F.wv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.re" "loc1:pasted__pasted__lambert4SG9F.re"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.of" "loc1:pasted__pasted__lambert4SG9F.of"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.r" "loc1:pasted__pasted__lambert4SG9F.ro"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.o" "loc1:pasted__pasted__lambert4SG9F.uv"
		;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.ofs" "loc1:pasted__pasted__lambert4SG9F.fs"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5.oc" "loc1:pasted__pasted__pasted__lambert4SG4.ss"
		;
connectAttr "loc1:groupId31.msg" "loc1:pasted__pasted__pasted__lambert4SG4.gn" -na
		;
connectAttr "groupId12.msg" "loc1:pasted__pasted__pasted__lambert4SG4.gn" -na;
connectAttr "loc1:polySurfaceShape1.iog.og[30]" "loc1:pasted__pasted__pasted__lambert4SG4.dsm"
		 -na;
connectAttr "polySurfaceShape12.iog.og[0]" "loc1:pasted__pasted__pasted__lambert4SG4.dsm"
		 -na;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG4.msg" "loc1:materialInfo12.sg"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5.msg" "loc1:materialInfo12.m"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5F.msg" "loc1:materialInfo12.t"
		 -na;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5F.oc" "loc1:pasted__pasted__pasted__lambert4SG5.c"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.c" "loc1:pasted__pasted__pasted__lambert4SG5F.c"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.tf" "loc1:pasted__pasted__pasted__lambert4SG5F.tf"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.rf" "loc1:pasted__pasted__pasted__lambert4SG5F.rf"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.s" "loc1:pasted__pasted__pasted__lambert4SG5F.s"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.wu" "loc1:pasted__pasted__pasted__lambert4SG5F.wu"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.wv" "loc1:pasted__pasted__pasted__lambert4SG5F.wv"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.re" "loc1:pasted__pasted__pasted__lambert4SG5F.re"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.of" "loc1:pasted__pasted__pasted__lambert4SG5F.of"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.r" "loc1:pasted__pasted__pasted__lambert4SG5F.ro"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.o" "loc1:pasted__pasted__pasted__lambert4SG5F.uv"
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.ofs" "loc1:pasted__pasted__pasted__lambert4SG5F.fs"
		;
connectAttr "loc1:polySurfaceShape1.o" "polySeparate1.ip";
connectAttr "polySeparate1.out[0]" "groupParts1.ig";
connectAttr "groupId1.id" "groupParts1.gi";
connectAttr "polySeparate1.out[1]" "groupParts2.ig";
connectAttr "groupId2.id" "groupParts2.gi";
connectAttr "polySeparate1.out[2]" "groupParts3.ig";
connectAttr "groupId3.id" "groupParts3.gi";
connectAttr "polySeparate1.out[3]" "groupParts4.ig";
connectAttr "groupId4.id" "groupParts4.gi";
connectAttr "polySeparate1.out[4]" "groupParts5.ig";
connectAttr "groupId5.id" "groupParts5.gi";
connectAttr "polySeparate1.out[5]" "groupParts6.ig";
connectAttr "groupId6.id" "groupParts6.gi";
connectAttr "polySeparate1.out[6]" "groupParts7.ig";
connectAttr "groupId7.id" "groupParts7.gi";
connectAttr "polySeparate1.out[7]" "groupParts8.ig";
connectAttr "groupId8.id" "groupParts8.gi";
connectAttr "polySeparate1.out[8]" "groupParts9.ig";
connectAttr "groupId9.id" "groupParts9.gi";
connectAttr "polySeparate1.out[9]" "groupParts10.ig";
connectAttr "groupId10.id" "groupParts10.gi";
connectAttr "polySeparate1.out[10]" "groupParts11.ig";
connectAttr "groupId11.id" "groupParts11.gi";
connectAttr "polySeparate1.out[11]" "groupParts12.ig";
connectAttr "groupId12.id" "groupParts12.gi";
connectAttr "polySphere1.out" "deleteComponent1.ig";
connectAttr "deleteComponent1.og" "deleteComponent2.ig";
connectAttr "polyTweak1.out" "polyTriangulate1.ip";
connectAttr "deleteComponent2.og" "polyTweak1.ip";
connectAttr "polyTriangulate1.out" "polyTweak2.ip";
connectAttr "polyTweak2.out" "deleteComponent3.ig";
connectAttr "deleteComponent3.og" "deleteComponent4.ig";
connectAttr "deleteComponent4.og" "deleteComponent5.ig";
connectAttr "deleteComponent5.og" "deleteComponent6.ig";
connectAttr "deleteComponent6.og" "deleteComponent7.ig";
connectAttr "deleteComponent7.og" "deleteComponent8.ig";
connectAttr "deleteComponent8.og" "deleteComponent9.ig";
connectAttr "loc1:blinn1SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:lambert2SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:lambert3SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:lambert4SG2.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__lambert4SG.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__lambert4SG2.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__lambert4SG4.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__lambert4SG6.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__lambert4SG8.pa" ":renderPartition.st" -na;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG4.pa" ":renderPartition.st" 
		-na;
connectAttr "pSphereShape1.iog" ":initialShadingGroup.dsm" -na;
connectAttr "loc1:blinn1SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:lambert4SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:lambert2SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:lambert3SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:lambert4SG3.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__lambert4SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__lambert4SG1.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__lambert4SG3.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__lambert4SG5.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__lambert4SG7.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__lambert4SG9.msg" ":defaultShaderList1.s" -na;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5.msg" ":defaultShaderList1.s"
		 -na;
connectAttr "loc1:blinn1SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:lambert4SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:lambert2SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:lambert3SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:lambert4SG3F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:pasted__lambert4SG1F.msg" ":defaultTextureList1.tx" -na;
connectAttr "loc1:pasted__pasted__lambert4SG1F.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "loc1:pasted__pasted__lambert4SG3F.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "loc1:pasted__pasted__lambert4SG5F.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "loc1:pasted__pasted__lambert4SG7F.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "loc1:pasted__pasted__lambert4SG9F.msg" ":defaultTextureList1.tx" -na
		;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5F.msg" ":defaultTextureList1.tx"
		 -na;
connectAttr "loc1:blinn1SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "loc1:lambert4SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "loc1:lambert2SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "loc1:lambert3SG1P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "loc1:lambert4SG3P2D.msg" ":defaultRenderUtilityList1.u" -na;
connectAttr "loc1:pasted__lambert4SG1P2D.msg" ":defaultRenderUtilityList1.u" -na
		;
connectAttr "loc1:pasted__pasted__lambert4SG1P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG3P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG5P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG7P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "loc1:pasted__pasted__lambert4SG9P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "loc1:pasted__pasted__pasted__lambert4SG5P2D.msg" ":defaultRenderUtilityList1.u"
		 -na;
connectAttr "defaultRenderLayer.msg" ":defaultRenderingList1.r" -na;
// End of loc1.ma