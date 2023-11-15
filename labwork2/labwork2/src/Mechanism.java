public class Mechanism {
    public void ledON(int ledNumber){
        Storage.ledOn(ledNumber);
    }

    public void ledsOff(){
        Storage.ledsOff();
    }

    public boolean switch1Pressed(){
        //todo for now returns false
        if(Storage.getSwitch1()==1) return true;
        return false;
    }

    public boolean switch2Pressed(){
        //todo for now returns false
        return false;
    }

    public boolean bothSwithcesPressed(){
        //todo for now returns false
        return false;
    }

    //Y e Z podem mover-se como task!!!!
    public void putPartInCell(){
        AxisZ axisZ = new AxisZ();
        AxisY axisY = new AxisY();
        axisZ.deliverPos();
        axisY.gotoPos(3);
        axisZ.returnPos();
        axisY.gotoPos(2);
    }

    public void takePartFromCell(){
        AxisZ axisZ = new AxisZ();
        AxisY axisY = new AxisY();
        axisY.gotoPos(3);
        axisZ.deliverPos();
        axisY.gotoPos(2);
        axisZ.returnPos();
    }
}
/*
 * Static methods do not use any instance variables of any object of the class they are defined in. 
 * Static methods take all the data from parameters and compute something from those parameters, with no reference to variables.
 */