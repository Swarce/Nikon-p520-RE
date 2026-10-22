import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
public class Decomp extends GhidraScript {
  public void run() throws Exception {
    String[] a=getScriptArgs();
    String out=a[0];
    DecompInterface di=new DecompInterface(); di.openProgram(currentProgram);
    PrintWriter pw=new PrintWriter(new FileWriter(out));
    FunctionIterator it=currentProgram.getFunctionManager().getFunctions(true);
    while(it.hasNext()){
      Function f=it.next();
      boolean want=a.length<2;
      for(int i=1;i<a.length;i++) if(f.getName().matches(a[i])) want=true;
      if(!want) continue;
      DecompileResults r=di.decompileFunction(f,120,monitor);
      pw.println("//==== "+f.getName()+" @ "+f.getEntryPoint());
      if(r.decompileCompleted()) pw.println(r.getDecompiledFunction().getC()); else pw.println("// FAILED "+r.getErrorMessage());
    }
    pw.close();
  }
}
