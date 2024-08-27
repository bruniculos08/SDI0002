/*
 * O servidor deve oferecer:
 * - Operações com matriz (implementando a interface IMatrix);
*/
import java.util.Random;
// import IMatrix;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;

public class ServerMatrix implements IMatrix {
    public ServerMatrix(){};

    public double[][] sum(double[][] a, double[][] b) throws RemoteException
    {
      int rows = a.length;
      int cols = a.length;

      if(rows != b.length || cols != b[0].length)
          throw new IllegalArgumentException(
          "Matrices must have the same sizes for sum.");

      double[][] c = new double[rows][cols];
      for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
          c[i][j] = a[i][j] + b[i][j];
      return c;
    }

  public double[][] mult(double[][] a, double[][] b) throws RemoteException
  {
    int a_rows = a.length;
    int a_cols = a[0].length;
    int b_rows = b.length;
    int b_cols = b[0].length;

    if(a_cols != b_rows)
          throw new IllegalArgumentException(
          "Invalid matrices dimensions.");

    double[][] c = new double[a_rows][b_cols];
    for(int i = 0; i < a_rows; i++)
      for(int j = 0; j < b_cols; j++)
      {
        c[i][j] = 0;
        for(int k = 0; k < a_cols; k++)
        {
          for(int l = 0; l < b_rows; l++)
            c[i][j] += a[i][k] * b[l][j];
        }
      }
    return c;
  }

  public double[][] randfill(int rows, int cols) throws RemoteException
  {
    Random rand = new Random();

    double[][] c = new double[rows][cols];
    for(int i = 0; i < rows; i++)
      for(int j = 0; j < cols; j++)
        c[i][j] = Math.random() * rand.nextInt();
    return c;
  }
  public static void main(String[] args) {
    try 
    {
      ServerMatrix server = new ServerMatrix();
      IMatrix stub = (IMatrix)UnicastRemoteObject.exportObject(server, 0);
      Registry reg = LocateRegistry.createRegistry(12344);
      reg.bind("matrix-server", stub);
      System.out.println("matrix working...");
    } 
    catch (Exception e) 
    {
      e.printStackTrace();
    }
  }
}
