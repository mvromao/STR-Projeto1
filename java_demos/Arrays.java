public class Arrays {    

    public void printArray(Object array[]){
        for(int i=0; i<array.length; i++) {
            if(i>0) 
				System.out.print(", ");
            System.out.print(array[i]);
        }    
        System.out.println();
    }
  
    public void printMatrix(Object matrix[][] ) {
        System.out.println("matrix={");
        for(int i=0;i<matrix.length;i++) {
            System.out.print(" ");
            printArray( matrix[i] );
        }
        System.out.println("}");        
    }
    
    static public void main(String args[]) {
        String array1[]={"what ", "is ","the ","matrix","?"};
        String array2[]={"you ", "must " ,"see ","it ","yourself","..."};
        String matrix[][] ={
            {"a","f","1","b"},
            {"d","5","2","8"},
            {"1","3","9","e"},
            {"f","a","c","a"}
        };
        Arrays d = new Arrays();
        d.printArray(array1);
        d.printArray(array2);
        d.printMatrix(matrix);                        
    }
}