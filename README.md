# FeerPong
F-klubbens overengineered og goldplated beerpong table!

For quite a while, two beautifully painted tables have been sitting in the dark corners of jægerstuen, ready to be turned into the nerdiest (and therefore also, coolest) beer pong tables known to man. This repository has therefore been created to conquer this stalemate and bring the dream to life. Currently, it is a bit empty in here, but in the future, we will collect everything relating to the FeerPong table in here. This will include, but not limited to, 2D files for the laser cutter that will engrave the table (already [here](PhysicalDesign/2DLogoLaserCutting), whoop!), code to the ontable-hardware and, of course, documentation for it all.

## FeerPong ideas:
- F-klubbens logo encompassing the beers ingraved into the tables via laser cutting
  - See [the 2D logo laser cutting files](PhysicalDesign/2DLogoLaserCutting)
- Diodes, sensors and ontable-hardware that can be used for various extensions of the traditional beer pong, truly bringing forth the era of FeerPong
  - Protocol ideas: (You are welcome to add more!)
    - Under_Score_Protocol
      - All active cups (cups on the table that has not yet been consumed) light up 
    - Holy_Grail_Protocol:
      - Randomly an active cup will light up for a set time. If that cup is hit the team owning the cup, recieve a penalty (drink an extra beer/shot, they also have to drink adjecent cups, End_Case_Protocol/Recursive_Protocol starts for them etc.)
    - Counter_Attack_Protocol:
      - When a cup is taken away (because the opposite team hit it) one or more cups on the opposite side light up for a short time. If the team downs the beer and then hit one of the cups, the oppsite team recieves a penalty (drink an extra beer/shot, they also have to drink adjecent cups, End_Case_Protocol/Recursive_Protocol starts for them etc.)
    - End_Case_Protocol/Recursive_Protocol:
      - When a cup is taken away (because the opposite team hit it) a timer starts. When the timer runs out a cup light up and if the team  haven't downed their beer before that, they have to drink that beer too, starting a new timer (this continues until you down a beer before the timer runs out or loose the game). The timer could be customisable 
  - Crazy new ideas: (Again, add more!)
    - Account creation and cross-game score keeping
    - App/web-app integration for game control, account management etc.
    

(You are also welcome to correct any spelling mistakes you find)
