public class Vector{
	private double x;
    private double y;
    private double z;

   public Vector(double x, double y, double z) {
        this.x = x;
        this.y = y;
        this.z = z;
    }
    public void sum(double x, double y, double z){
        this.x+= x;  // this.x = this.x + x;
        this.y+= y;
        this.z+= z;
    }
    public Vector scalarProduct(Vector v) {
       Vector result = new Vector(0,0,0);
       result.x = this.x * v.x;
       result.y = this.y * v.y;
       result.z = this.z * v.z;
       return result; 
    }
    public double module() {       
       double module = Math.sqrt(  Math.pow(this.x,2)+Math.pow(this.y,2)+Math.pow(this.z,2)  );       
       return module;
    }
    public String toString() {
           String s = new String(" "+this.x+"x + "+this.y+"y + "+this.z+"z");
        return s;
    } 
}
