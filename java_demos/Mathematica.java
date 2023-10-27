public class Mathematica {

    public Mathematica() {
        System.out.println("Put initialization code here");
    }
    static public void main(String args[]) {

        Vector v = new Vector(2,3,5);

        System.out.println("Vector v =" + v.toString()); 
        System.out.println("Module of v ="  +v.module());
       
		Vector v2 = new Vector(1,4,2);
        Vector v3 = v.scalarProduct(v2);

        System.out.println("Vector v3 =" + v3); 
        System.out.println("Module of v3 =" + v3.module() );
    }    
}
 