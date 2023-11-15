//import java.util.Scanner;

public class App {
    public static void main(String[] args) throws Exception {
        System.out.println("Labwork2 from Java");

        Storage.initializeHardwarePorts();

        SystemThread thread = new SystemThread();
        thread.start();
    }
}

    //     AxisX axisX = new AxisX();
    //     AxisY axisY = new AxisY();
    //     AxisZ axisZ = new AxisZ();
    //     Mechanism mechanism = new Mechanism();

    //     Storehouse storehouse = new Storehouse();
    //     storehouse.cleanAll();
    //     calibrate();

    //     int op = -1;
    //     Scanner scan = new Scanner(System.in);
    //     while(op != 0){
    //         imprimeMenu();
    //         //System.out.println("Enter an option:");
    //         op = scan.nextInt();
    //         switch(op){
    //             case 1:
    //                 Scanner pos = new Scanner(System.in);
    //                 System.out.println("X=");
    //                 int c = pos.nextInt();
    //                 System.out.println("Z=");
    //                 int v = pos.nextInt();
    //                 if (c >= 1 && c <= 3 && v >= 1 && v <= 3) {
    //                     if (storehouse.isCellFull(c-1, v-1)!=1){
    //                         pos.nextLine(); // Limpa o buffer do scanner
    //                         System.out.print("Insira o pallet Name: ");
    //                         String palletName = pos.nextLine();
    //                         System.out.print("Insira o código de referência: ");
    //                         String referenceCode = pos.nextLine();
    //                         System.out.print("Insira a data de envio: ");
    //                         String shipmentDate = pos.nextLine();
    //                         storehouse.insertCell(c-1, v-1, 'X',palletName, referenceCode, shipmentDate);
    //                         DeliveryThread deliveryThread = new DeliveryThread(c, v);
    //                         deliveryThread.start(); 
    //                         try {
    //                             deliveryThread.join();
    //                         } catch (InterruptedException e) {
    //                             System.out.println("Thread interrupted.");
    //                         }
    //                     } else{
    //                         System.out.println("Position occupied!");
    //                     }
    //                 } else {
    //                     System.out.println("Posição inválida.");
    //                 }
    //                 break;
    //             case 2: 
    //                 calibrate();
    //                 break;
    //             case 3:
    //                 Scanner scanner = new Scanner(System.in);
    //                 char input;
    //                 do {
    //                     System.out.println("Fullness:");
    //                     storehouse.showStorehouse();
    //                     System.out.println("Enter 'x' to exit:");
    //                     input = scanner.next().charAt(0);
    //                 } while (input != 'x');
    //                 break;
    //             case 4:
    //                 //axisZ.deliverPos();
    //                 mechanism.putPartInCell();
    //                 break;
    //             case 5:
    //                 mechanism.takePartFromCell();
    //                 break;
    //             case 6:
    //                 break;
    //             case 7: 
    //                 System.out.println("Para 1, 2 ou 3?");
    //                 Scanner j = new Scanner(System.in);
    //                 String input7 = j.nextLine();
    //                 char m = input7.charAt(0);
    //                 int i = Character.getNumericValue(m);
    //                 axisX.gotoPos(i);
    //                 break;
    //             case 8: 
    //                 System.out.println("Para 1, 2 ou 3?");
    //                 Scanner h = new Scanner(System.in);
    //                 String input8 = h.nextLine();
    //                 char x = input8.charAt(0);
    //                 int z = Character.getNumericValue(x);
    //                 axisY.gotoPos(z);
    //                 break;
    //             case 9: 
    //                 System.out.println("Para 1 a 3 (Só down)?");
    //                 Scanner k = new Scanner(System.in);
    //                 String input9 = k.nextLine();
    //                 char p = input9.charAt(0);
    //                 int l = Character.getNumericValue(p);
    //                 axisZ.gotoPos(l);
    //                 break;
    //                 /*
    //                 return 10; // position z = 1 up   
    //                 return 1; // position z = 1 down 
    //                 return 20; // position z = 2 up 
    //                 return 2; // position z = 2 down 
    //                 return 30; // position z = 3 up 
    //                 return 3; // position z = 3 down 
    //                  */
    //             default: break;
    //         }
    //     }
    //     scan.close();
    // }

    // //"public" methods and variables can be accessed from anywhere in the program, while 
    // //"private" methods and variables can only be accessed within the class in which they are defined.
    // //"method" is a block of code that performs a specific task.
    
    // static public void calibrate(){

    //     System.out.println("\nCalibrating... \n\n");

    //     Axis axisX = new AxisX();
    //     Axis axisY = new AxisY();
    //     Axis axisZ = new AxisZ();

    //     CalibrationThread threadY = new CalibrationThread(axisY);
    //     threadY.start();
    //     try{
    //         threadY.join();
    //     } catch (InterruptedException e) {
    //     System.out.println("Thread interrupted.");
    //     }

    //     CalibrationThread threadX = new CalibrationThread(axisX);
    //     CalibrationThread threadZ = new CalibrationThread(axisZ);
    //     threadX.start();
    //     threadZ.start();
    //     try {
    //         threadX.join();
    //         threadZ.join();
    //     } catch (InterruptedException e) {
    //         System.out.println("Thread interrupted.");
    //     }
    //     System.out.println("Calibrated \n\n");
    //     /*AxisX axisX = new AxisX();
    //     AxisY axisY = new AxisY();
    //     AxisZ axisZ = new AxisZ();
    //     axisX.gotoPos(1);
    //     axisY.gotoPos(2);
    //     axisZ.gotoPos(1);*/
    // }

    // static private void imprimeMenu() {
    //     System.out.println("\u001B[36mPlease select an option:\u001B[0m");
    //     System.out.println("\u001B[32m(1) - Store a pallet in a specific x, z position\u001B[0m");
    //     System.out.println("\u001B[33m(2) - Recalibrate\u001B[0m");     
    //     System.out.println("\u001B[35m(3) - Show Storehouse\u001B[0m");
    //     System.out.println("\u001B[31m(4) - TESTE1\u001B[0m");
    //     System.out.println("\u001B[31m(5) - TESTE2\u001B[0m");
    //     System.out.println("\u001B[31m(6) - TESTE3\u001B[0m");
    //     System.out.println("\u001B[31m(7) - gotoX\u001B[0m");
    //     System.out.println("\u001B[31m(8) - gotoY\u001B[0m");
    //     System.out.println("\u001B[31m(9) - gotoZ\u001B[0m");
    //     //System.out.println("\u001B[34m(X) - Exit the program\u001B[0m");
    // }
    // /*
    //  \u001B[31m: Red
    // \u001B[32m: Green
    // \u001B[33m: Yellow
    // \u001B[34m: Blue
    // \u001B[35m: Purple
    // \u001B[36m: Cyan
    //  */

//}


