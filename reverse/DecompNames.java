import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;
public class DecompNames extends GhidraScript {
  public void run() throws Exception {
    String[] a=getScriptArgs();
    DecompInterface di=new DecompInterface(); di.openProgram(currentProgram);
    PrintWriter pw=new PrintWriter(new FileWriter(a[0]));
    for(int i=1;i<a.length;i++){
      Function f=null;
      if(a[i].startsWith("0x")) f=getFunctionContaining(toAddr(Long.parseLong(a[i].substring(2),16)));
      else { for(Symbol s: currentProgram.getSymbolTable().getSymbols(a[i])){ f=getFunctionAt(s.getAddress()); if(f!=null) break; } }
      if(f==null){ pw.println("//==== "+a[i]+" NOT FOUND"); continue; }
      DecompileResults r=di.decompileFunction(f,180,monitor);
      pw.println("//==== "+f.getName()+" @ "+f.getEntryPoint());
      if(r.decompileCompleted()) pw.println(r.getDecompiledFunction().getC()); else pw.println("// FAILED "+r.getErrorMessage());
    }
    pw.close();
  }
}
