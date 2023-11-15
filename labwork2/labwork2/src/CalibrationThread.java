//package str; //a package is a way to organize related classes and interfaces.

public class CalibrationThread extends Thread {
    private final Axis axis;
    //"private" keyword indicates that the variable can only be accessed within the class in which it is declared.
    //"final" keyword indicates that the variable can only be assigned once and cannot be changed later.

    public CalibrationThread(Axis axis){
        this.axis = axis;
    }

    public void initializeCalibration(){
        if(axis instanceof AxisY){
            axis.gotoPos(2);
        }
        else{
            axis.gotoPos(1);
        }
    }
    public void run(){
        //axis.gotoPos(2);
        this.initializeCalibration();
    }
}

/*
The following are some of the methods provided by the Thread class:

start(): Starts the execution of the thread.
run(): Contains the code that is executed when the thread is started.
sleep(): Causes the currently executing thread to sleep for a specified number of milliseconds.
join(): Waits for the thread to die.
yield(): Causes the currently executing thread to yield to another thread.

Here is an example of how to use the Thread class:

public class MyThread extends Thread {
    public void run() {
        System.out.println("MyThread is running.");
    }
}

public class Main {
    public static void main(String[] args) {
        MyThread myThread = new MyThread();
        myThread.start();
    }
}

In the above example, we create a new class MyThread that extends the Thread class. We override the run() method to
 define the code that will be executed when the thread is started. In the main() method, we create an instance of MyThread and call the start()
  method to start the execution of the thread.
 */