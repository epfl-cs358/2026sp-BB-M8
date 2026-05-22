# 2026sp-BB-M8
<!-- TODO: Picture/Video of the final BB-M8 -->


## Contributors

* [Bernadette Orglmeister](https://github.com/boerniii)
* [Noa Cuccodoro](https://github.com/noa-ccdr)
* [Gaspard Héliot](https://github.com/gheliot)
* [Khadija Kammoun](https://github.com/khka5)
* [Gaël Diep](https://github.com/Damedane)
* [Jérôme](https://github.com/Jess-lnga) <!-- TODO:  -->



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
The head is divided into different elements: the lid, the bottomplate (holding the magnets), the top plate (where the components are attached) and the manchette (hiding the bottom plate). 

1. Print all the 3D printing parts with PETG filament. <!-- TODO: More specifications -->

3. Bottom plate:
    - Add the **tree inserts**.
    - Screw the gear to it. 
4. ESP32 Holder:
    - Add tree inserts into the holes
    - Then glue it into the lid, so that the holdes for the camera lines up.
5. Lid: 
    - Screw the two top parts together.
    - Then screw it to the rest of the lid.
    - Glue the samll magnets into the holes

6. Top Plate:
    - Laser cut the parts using **4mm MDF**.
    - Glue the parts together.
    - Glue small magnets into the four holes at the edge
    - Glue a small ball bearing into the hole in the middle, **pay attention that no glue drips into the bearing!**
    - Once the [electronic assembly](#electronic-assembly-of-the-head) is done, screw it onto the plate.
        - Before attatching the stepper, make sure to add the gear.
    - Add bit of velcro to the LIPO and the counterpart to the plate.

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
