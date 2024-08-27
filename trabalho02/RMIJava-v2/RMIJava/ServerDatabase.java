/*
 * O servidor deve oferecer:
 * - Operações com a base de dados (implementando IDatabase)
  */
// import RMI.IDatabase;
import java.rmi.*;
import java.rmi.registry.*;
import java.rmi.server.*;
import java.io.IOException;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.nio.file.Files;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.stream.Collectors;
import java.util.stream.IntStream;
import java.util.Scanner;

public class ServerDatabase implements IDatabase{
    public ServerDatabase () {};

    public void save(double[][] a, String filename) throws RemoteException
    {
      File obj = new File(filename);
      System.out.println("File created.");
      
      try 
        {
          FileWriter FW = new FileWriter(filename);
          String str;
          for(int i = 0; i < a.length; i++)
          {
            for(int j = 0; j < a[0].length; j++)
            {
              str = a[i][j]+"";
              FW.write(str);
              FW.write(";");
            }
            FW.write("\n");
          }
          FW.close();
        }
      catch(IOException e)
        {
          System.out.println("Error.");
          e.printStackTrace();
        }
    }

    public double[][] load(String filename) throws RemoteException
    {
      try 
      {
        String text = "";
        try {
          str = new String(Files.readAllBytes(Paths.get(file_path)));
        }
        catch(IOException e)
        {
          e.printStackTrace();
        }


        String[] lines = text.split("\n");

        int rows = lines.length;
        int cols = (lines[0].split(" ")).length;

        double[][] c = new double[rows][cols];

        for(int i = 0; i < rows; i++)
        {
          String[] numbers = lines[i].split(" ");
          for(int j = 0; j < cols; j++)
          {
            c[i][j] = Double.parseDouble(numbers[j]);
          }
        }
        return c;
      }
      catch (IOException e)
      {
        e.printStackTrace();
      }
      return null;
    }

    public void remove(String filename) throws RemoteException
    {
      File obj = new File(filename);
      if(obj.delete())
        System.out.println("File {" + obj.getName() + "} deleted.");
      else
        System.out.println("Error on deleting file.");
    }

    public static void main(String[] args)
    {
      try
      {
        ServerDatabase server = new ServerDatabase();
        IDatabase stub = (IDatabase)UnicastRemoteObject.exportObject(server, 0);
        Registry reg = LocateRegistry.createRegistry(12345);
        reg.bind("db-server", stub);
        System.out.println("db working...");
      }
      catch (Exception e)
      {
        e.printStackTrace();
      }
    }
}
