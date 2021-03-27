package cvc;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.List;

class Utils
{
  static final String osName = System.getProperty("os.name");

  static
  {
    loadLibraries();
  }

  /**
   * @return the absolute paths for cvc and cvcJavaApi dynamic libraries
   */
  private static List<String> getLibraryPaths() throws IOException
  {
    List<String> names = new ArrayList<>();
    if (osName.startsWith("Linux"))
    {
      names.add("libcvcjni.so");
      names.add("libcvc4.so.7");
    }
    else if (osName.startsWith("Mac"))
    {
      names.add("libcvcjni.dylib");
      names.add("libcvc4.dylib");
    }
    else if (osName.startsWith("Windows"))
    {
      names.add("cvcJavaApi.dll");
      names.add("cvc4.dll");
    }
    else
    {
      throw new RuntimeException("The operating system '" + osName + "' is not supported");
    }

    List<String> paths = new ArrayList<>();
    for (String name : names)
    {
      String path = System.getProperty("java.io.tmpdir") + File.separatorChar + name;
      if (Files.exists(Path.of(path)))
      {
        // return if the library exists in the temp directory
        // return cvcApiLibFile; // TODO (wishue  #83): this is disabled for development.
        // Enable this before release
      }
      // copy the library from resources to the temp directory
      InputStream input = Solver.class.getResourceAsStream("/" + name);
      Files.copy(input, Paths.get(path), StandardCopyOption.REPLACE_EXISTING);
      paths.add(path);
    }

    return paths;
  }

  /**
   * load cvc jni library
   */
  public static void loadLibraries()
  {
    try
    {
      List<String> paths = getLibraryPaths();
      for (String path : paths)
      {
        System.load(path);
      }
    }
    catch (IOException e)
    {
      e.printStackTrace();
    }
  }
}
