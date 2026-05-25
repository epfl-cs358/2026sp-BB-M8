# 2026sp-BB-M8
<!-- TODO: Picture/Video of the final BB-M8 -->


## Contributors

* [Bernadette Orglmeister](https://github.com/boerniii)
* [Noa Cuccodoro](https://github.com/noa-ccdr)
* [Gaspard Héliot](https://github.com/gheliot)
* [Khadija Kammoun](https://github.com/khka5)
* [Gaël Diep](https://github.com/Damedane)
* [Jérôme Essola Elanga](https://github.com/Jess-lnga)



## Table of Content



- [Project overview](#project-overview)
    - [About the BB-M8](#about-the-bb-m8)
    - [List of Materials](#list-of-materials)
- [Built the BB-M8](#built-the-bb-m8)
    - [Head](#head)
    - [Inner Body](#inner-body)
    - [Sphere](#sphere)
- [Controlling the BB-M8](#controlling-the-bb-m8)


## Project overview
### About the BB-M8

<!-- TODO: 
- Talk about the project 
- Maybe add futur developpment possibilities
-->

### List of Materials
Here is a exhausive list of all the elements needed to built the BB-M8. Note that you will need a 3D-printer with PETG filament to print some parts and a laser cutter.

#### Buy:
| Electronic Component | Model | Quantity |
|-----------|---------------------------|-----|
| Microcontroller | Wemos D1 R32 ESP32 | 1 |
| Microcontroller | AIThinker ESP32-CAM | 1 |
| Buck Converter | LM2596 | 2 |
| Stepper Motor| 28BYJ-48 | 1 |
| Driver Motor | ULN2003 | 1 |
| Servo Motor | ----- | 1 |
| Stepper Motor | Nema 17 | 1 |
| Motor Driver | A4988 | 1 |
| BMS 3S | ------- | 1 |
| BMS 2S | 2S 20A 18650 Lithium Battery Protection Board | 1 |
| LIPO | ---- Body | 1 |
| LIPO |LiPo Akku LEMONRC 350 - 7.4V (35C) | 1 |
| ON / OFF Switch | ------- | 2 |
| Speaker | ----- | 1 | 
| Speaker Thingy | ---- | 1 | 
| Gearbox | ---- | 1 |
| IMU | ------ | 1 |

<!-- TODO: Remove Speaker components if not used -->

| Other Parts | Specifications | Quantity |
|-----------|---------------------------|-----|
| Aluminium Bar | ----- | 1 |
| Big Ball Bearings | ------ Diameter, ect. | 2 |
| Small Ball Bearing | 3 x 10 x 4 mm | 1 | <!-- TODO: What bearing is in the swinger -->
| Small Magnets | 5mm diameter, 4 mm hight | 8 |
| Big Neodymium Magnets | 20 mm diameter, 12 mm hight | 4 |
| Magnets for Bottom of Head | 14 mm diameter, 18 mm hight | 4 |
| Wheels | 13 mm diameter | 4 |

<!-- TODO: Add any missing components-->


#### 3D print:

<!-- TODO: Uplode the stl file, and make a refernce to them -->

#### Laser cut:
<!-- TODO: Uplode the dfx file, and make a refernce to them -->



## Built the BB-M8

### Head
<!-- TODO: Foto of the entire head -->
<!-- TODO: Describtion of the head and its functioning -->
The head contains the ESP32 with a camera and has an internal rotation mechanism, to make it turn. An other important aspect of the head is it's iconic look and it's size relative to the sphere, that makes our BB-M8 resemble the original one. 

#### Building the mechanical part:
The head is divided into different elements: the lid, the bottom plate (holding the magnets), the top plate (where the components are attached) and the manchette (hiding the bottom plate). 


1. Bottom plate:
    - Print the [bottom Plate](3D-Files/stl/Head_BottomPlate.stl) with PETG filament.
    - Add the **tree inserts**.
    - Screw the [gear](3D-Files/stl/Head_GearBottomPlate.stl) to it.
    - Put the magnets and the wheels into the holes.
    - 🫟 The bottom plate and the gear can be printed in one piece. 

5. Lid: 
    - Print the [bottom of the lid](3D-Files/stl/Head_BottomPartLid.stl) and two times the [top of the lid](3D-Files/stl/Head_TopPartLid.stl).
    - Screw the two top parts together.
    - Then screw it to the rest of the lid.
    - Glue the samll magnets into the holes. 
    - 🫟 The holes where the top is connected to the bottom do not perfectly line up, so if you want to have it more beautiful, change it.
    - 🫟 Theoretically, the lid could be printed in one piece with the front side of the ESP32 Holder directly attached to it. 

4. ESP32 Holder:
    - Print the two parts of the ESP32 Holder, [front](3D-Files/stl/Head_ESP32HolderFrontSide.stl) and [back](3D-Files/stl/Head_ESP32HolderBackSide.stl).
    - Add tree inserts into the holes.
    - Then glue it into the lid, so that the holdes for the camera line up.
     
6. Top Plate:
    - Laser cut the [top plate](3D-Files/dfx/Head_TopPlate.dxf) with the [stepper holder](3D-Files/dfx/Head_TopPlateStepper.dxf) and two times its [side](3D-Files/dfx/Head_TopPlateSides.dxf) using **4mm MDF**.
    - Glue the parts together.
    - Glue small magnets into the four holes at the edge.
    - Glue a small ball bearing into the hole in the middle, **pay attention that no glue drips into the bearing!**
    - Once the [electronic assembly](#electronic-assembly-of-the-head) is done, screw it onto the plate.
        - Make sure that the screws don't interfer with the magnets and wheels of the bottom plate.
        - Before attatching the stepper, make sure to print and add the [gear](3D-Files/stl/Head_GearForStepper.stl) and also depending on the size of the screw head, carf the edges so that they are not in the way of the gear.
    - Add bit of velcro to the LIPO and the counterpart to the plate.

7. Final Assembly:
    - Print and then glue the [ball bearing stopper](3D-Files/stl/Head_BallBearingStopper.stl) to a small bar (that fits the ball bearing perfectly). Then slide it through the ball bearing and into the gear of the bottom plate, line it up with the other gear and the attach it with a screw. 
    - 🫟 The attachement of the bearing can be done more properly by using a rings and creating a shaft. <!-- TODO: Better explenation -->

8. Manchette:
    - Print the [manchette](3D-Files/stl/Head_Manchette.stl)
    - Add four M2.5 inserts into the four holes.
    - Screw it to the top plate.

Now the head can be easily opend and closed with the magnets!


<!-- TODO: Continue-->

#### Electronic Assembly of the head:
<!-- TODO: Add scema of the electronic assembly !!!!!!! add the BMS -->


1. Calibrate the **buck convertor to 5V**.
2. Solder the connector of the LIPO to the BMS and make sure that is works correctly.
3. Connect the components as in the scematic above. 
    - Make sure that the **cables connecting the ESP32 are long** enough to easily open the lid of the head. 






### Inner Body

### Sphere

## Controlling the BB-M8
