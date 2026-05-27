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



- [2026sp-BB-M8](#2026sp-bb-m8)
  - [Contributors](#contributors)
  - [Table of Content](#table-of-content)
  - [Project overview](#project-overview)
    - [About the BB-M8](#about-the-bb-m8)
    - [List of Materials](#list-of-materials)
      - [Buy:](#buy)
      - [3D print:](#3d-print)
      - [Laser cut:](#laser-cut)
  - [Built the BB-M8](#built-the-bb-m8)
    - [Head](#head)
      - [Building the mechanical part:](#building-the-mechanical-part)
      - [Electronic Assembly of the head:](#electronic-assembly-of-the-head)
    - [Axis](#axis)
      - [Building the mechanical part:](#building-the-mechanical-part-1)
    - [Main Body](#main-body)
      - [Building the mechanical part:](#building-the-mechanical-part-2)
    - [Sphere](#sphere)
  - [Controlling the BB-M8](#controlling-the-bb-m8)
    - [Flashing the code](#flashing-the-code)
    - [Steering the BB-M8](#steering-the-bb-m8)


## Project overview
### About the BB-M8
The BB-M8 is a spherical robot capable of moving across flat surfaces. This is achieved by means of an axis fitted with a pendulum, which can shift the centre of gravity, enabling the robot to navigate curves and maintain stability. The sphere also features a head, which is attached to the body by magnets and always remains on the upper surface of the sphere. The head is equipped with a camera and a rotating mechanism, allowing it to look in all directions. 

The BB-M8 can be connected to any computer via Wi-Fi and controlled from there via an interface, where you can also view the live feed from the camera.

Our BB-M8 is a replica of BB-8 from Star Wars.
The body has a diameter of approximately 35 cm, so a bit smaller than the original one.



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
| BMS 3S | HW-544 3S (11.1V/12.6V) BMS | 1 |
| BMS 2S | 2S 20A 18650 Lithium Battery Protection Board | 1 |
| LIPO | Lipo 11.1V (40C) | 1 |
| LIPO |LiPo Akku LEMONRC 350 - 7.4V (35C) | 1 |
| ON / OFF Switch | ------- | 2 | 
| Gearbox | ---- | 1 |
| IMU | ------ | 1 |
| USB-to-TLL serial adapter | HW-417-V1.2 | 1 |


| Other Parts | Specifications | Quantity |
|-----------|---------------------------|-----|
| Aluminium Bar | ----- | 1 |
| Big Ball Bearings | ------ Diameter, ect. | 2 |
| Small Ball Bearing | 3 x 10 x 4 mm | 1 | <!-- TODO: What bearing is in the swinger -->
| Small Magnets | 5mm diameter, 4 mm hight | 8 |
| Big Neodymium Magnets | 20 mm diameter, 12 mm hight | 4 |
| Magnets for Bottom of Head | 14 mm diameter, 18 mm hight | 4 |
| Wheels | 13 mm diameter | 4 |

- You will also need a **USB Isolator** to protect you computer and **data cables** to flash the code. 

- To charge the LIPOs, you also need access to a **battery charging station**.
- Screws, nuts, inserts, jumpers and other basic supply is also needed, as well as tape and glue.


#### 3D print:
Here you can find all the [stl files](3D-Files/stl) that need to be printed with PETG filament. You find further information about the quantites in the specific subparts of building the BB-M8.

<!-- TODO: Uplode the stl file, and make a refernce to them -->

#### Laser cut:
The BB-M8 has also a few parts made of MDF. Here are the [dxf files](3D-Files/dxf). Note that you will need to use **different thicknesses** for the head and the body! Further information can be found in the corresponding section.
<!-- TODO: Uplode the dfx file, and make a refernce to them -->



## Built the BB-M8
Here are the instruction for building the BB-M8. Note that points having 🫟 in front of it, are further suggestens to make it cleaner and/or to remove unessecary holes, for example, that are no longer needed.

### Head
<!-- TODO: Foto of the entire head -->
<!-- TODO: Describtion of the head and its functioning -->
The head contains the ESP32 with a camera and has an internal rotation mechanism, to make it turn. An other important aspect of the head is it's iconic look and it's size relative to the sphere, that makes our BB-M8 resemble the original one. 

<p align="center">
  <img src="Images/head.jpeg" width="50%" />
  <img src="Images/head_bottom.jpeg" width="41.5%" />
</p>


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

<p align="center">
    <img src="Images/BMS_head.jpeg" width="24%" />
    <img src="Images/head_bottomplate.jpeg" width="26%" />
    <img src="Images/Screwing_the_head.jpeg" width = "16.5%" />
    <img src="Images/Screwing_the_servo_head.jpeg" width="18%" />
</p>

<p align="center">
    <img src="Images/head_top_plate.jpeg" width="50%" />
    <!-- TODO: Image of the inside of the lid with EPS32 CAM-->
</p>





<!-- TODO: Continue-->

#### Electronic Assembly of the head:
<!-- TODO: Add scema of the electronic assembly !!!!!!! add the BMS -->


1. Calibrate the **buck convertor to 5V**.
2. Solder the connector of the LIPO to the BMS and make sure that it works correctly.
3. Connect the components as in the scematic above. 
    - Make sure that the **cables connecting the ESP32 are long** enough to easily open the lid of the head. 


### Axis
The Axis is the part that will transmitt the rotation from the stepper to the sphere. It is made of aluminum to make sure it doesn't bend, it also holds a gear that should be making the axis turn, 2 bearings that will hold the main body and avoid friction, 4 stop rings that will make sure the bearings stay in place, and the box doesn't slip out of the bearing, and finaly a junction in 2 parts that will be the attachement of the axis to the body.  

![Axis Illustration](Images/Axis_illustration.png)


#### Building the mechanical part:
The axis is divided into different elements: the rode, the gear, the bearings, the stop rings, the first junction, and the second junction. 


1. Rode:
    - Get yourself a 310mm aluminum rode with diameter 10.
    - Drill holes and thread them with a lathe at the two ends of the rode for M2.5 Screws. The holes should be around 5mm deep
    - Make a 10mm D shape that is 1mm deep at the 2 ends, and make sure the 2 ends have D shapes facing the exact same direction.
    - 🫟 The deepness of the holes, and the lenght of the D shapes can be a bit bigger and it will totally work. Just don't make them smaller.
    - 🫟 If you need visual support, [here](3D-Files/stl/Axis_Rode.stl) is an STL file, but DO NOT PRINT IT, it won't be resistant enough!  

2. Gear: 
    - 3D Print the [gear](3D-Files/stl/Axis_GearMainPart.stl) and two times the [second part of the gear](3D-Files/stl/Axis_GearSecondPart.stl).
    - With 4 M2.5x16 screws and 4 M2.5nuts, tighten the gear around the axis.
    - 🫟 For now do not tighten them to the max so that you can easily move it around and find the right positio nonce the main body is done.
    - 🫟 Keep in mind that all the axis will have to be disassembled and reassembled in to the actual main body, so this is just to test and make sur all parts work.

3. Bearings:
    - Buy two bearings of size 35x10x11.
    - You'll put them on the rode on each side after adding the two first stop rings (next step).
     
4. Stop rings:
    - 3D print 4 times the [stop rings](3D-Files/stl/Axis_StopRing.stl).
    - With 4 M2.5x16 screws and 4 M2.5nuts, tighten the stop rings around the axis.
    - Remember to follow the first screen shot to put everything in the right order.

5. First junction:
    - 3D print 2 times the [first junction](3D-Files/stl/Axis_JunctionWithSphere.stl).
    - Put it in the D shape of each ends, and screw it to the bar with an M2.5x10  
    - 🫟 If you cannot find a screw that perfectly tighten the junction, just add nuts until it has the desired lenght (and very the lenght of the screw) 

6. Second junction:
    - 3D print 2 times the [second junction](3D-Files/stl/Axis_JunctionToJunction.stl).
    - Just clip it on the first junction.
    

Now the Axis is done, you'll have to disassemble it and reassemble it in the main body once you're sure each component of the axis is well made!


### Main Body
The main body is essentially a wooden box with a few holes to put a Servo, and has the importent duty to hold the axis, the nema, the poll for the head, and the swinger. It is the centerpiece of the project.  
<p align="center">
    <img src="Images/MainBody_illustration1.png" width="33%" />
    <img src="Images/MainBody_illustration2.png" width="33%" />
    <img src="Images/MainBody_illustration3.png" width="33%" />
</p>

#### Building the mechanical part:
The main body is divided into multiple parts: the box, the IMU holder, the two nema holders, the stepper and its gearbox, the Servo, and the pin that holds the second arm of the swinger.


1. Box:
- On an 8mm thickness sheet of MDF, laser cut one [Large side](3D-Files/dxf/SideBoxLarge.dxf), one [Large side with servo](3D-Files/dxf/SideBoxLargeServoHole.dxf), and two [Sides with holes for axis](3D-Files/dxf/SideBoxWithHoleForAxis.dxf).
- then be carefull to glue everything together with 90° angles (easier said than done!).

1. IMU holder: 
    - 3Print the [IMU holder](3D-Files/stl/MainBody_IMUHolder.stl)
    - put it into place with 2 M3x10 screws and 2 M3 nuts 

2. Nema holders:
    - 3D print one [Nema holder Servo side back](3D-Files/stl/MainBody_NemaHolderAttachementServoSide.stl), one [Nema holder Cap side back](3D-Files/stl/MainBody_NemaHolderAttachementCapSide.stl) and one [Nema holder](3D-Files/stl/MainBody_NemaHolder.stl). All of these three will attache the nema to the center of the box. First assemble this.
    - Then the next three part are going to prevent the Nema's shaft from bending and loosing the contact between the two gears.
    - 3D print two [Nema holder front](3D-Files/stl/MainBody_NemaHolderShaft.stl)and one [Nema holder front screwed](3D-Files/stl/MainBody_NemaHolderScrewed.stl). Assemble these last three pieces together. 
     
3. Stepper & Gearbox:
    - Assemble the Nema117 with the Gearbox, and make sure the Nema Shaft is tightened on the gearbox input.
    - Assemble the Nema17 with the Nema holders using 9 M2.5x10 for the holders themselves, and 9 M3x12 to connect the holders on the MDF box.
    - 🫟 Make sure the socket of the Nema is looking down.

4. Servo:
    - Add the Servo in his hole. Make sure its cables are looking down. 
    - Screw it with 4 M3x12 screws and 4 M3 nuts.  

5. Pin:
    - Buy a pin that is 3mm diameter and 40mm long.
    - Put it inside the hole that is facing the Servo on the opposite side of the box.
    - 🫟 This pin will hold the second arm of the Swinger 
    

Now the Main Body is done, let's build the Swinger!




### Sphere
The sphere itself is 3D-printed in multiple smaller parts so that it can easily be printed on a normal sized Prusa. You need to print the following parts in PETG filament:

| stl file | Quantity |
|-----------|------------|
| [caps](3D-Files/stl/Sphere_Cap.stl) | 2 |
| [bones](3D-Files/stl/Sphere-Bone.stl) | 8 |
| [equator piece](3D-Files/stl/Sphere_Equator.stl)| 4 |
| [top yellow piece](3D-Files/stl/Sphere-TopYellowPart.stl) | 8 |
| [white parts](3D-Files/stl/Sphere_WhitePart.stl) | 6 |
| [opening lid](3D-Files/stl/Sphere_Lid.stl) | 2 |
| [lid surrounding](3D-Files/stl/Sphere_LidSurrounding.stl) | 2 |
| [junction with axis](3D-Files/stl/sphere_axisJunction.stl) | 2 |

Once all parts are printed, glue the pieces together. It is important to keep **1/8 open** so that the inner body still can be placed inside. Also **do not** glue the caps to the rest of the sphere. 

#### Builting the sphere
 1. 3D-Print all the parts.
 2. Glue the pieces together with hot glue.
    - It is important to keep **1/8 open** so that the inner body still can be placed inside.
    - **Do not** glue the caps to the rest of the sphere. 
 
3. Place the [main body](#main-body) inside and make sure that it has enough space.
4. Put the second cap on top, close the last part and attach it with tape.




<p align="center">
    <img src="Images/Sphere_open.jpeg" width="50%" />
</p>

<!-- TODO: Photo of closed sphere-->

## Controlling the BB-M8

### Flashing the code

### Steering the BB-M8



