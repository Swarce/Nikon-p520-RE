import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import java.io.*;
public class ApplySyms extends GhidraScript {
  public void run() throws Exception {
    String path=getScriptArgs()[0];
    BufferedReader br=new BufferedReader(new FileReader(path));
    String l; int n=0,f=0;
    long lo=0xc0000800L, hi=0xc0000800L+0x14e058L;
    while((l=br.readLine())!=null){
      String[] p=l.trim().split("\\s+",3); if(p.length<3) continue;
      long a=Long.parseLong(p[0],16);
      if(a<lo||a>=hi) continue;
      Address ad=toAddr(a);
      String nm=p[2].replaceAll("[\\s]","_");
      try{ createLabel(ad,nm,true,SourceType.IMPORTED); n++; }catch(Exception e){}
      if(p[1].equalsIgnoreCase("T") && (a&3)==0){
        if(getFunctionAt(ad)==null){ disassemble(ad); try{ createFunction(ad,nm); f++;}catch(Exception e){} }
      }
    }
    println("labels "+n+" funcs "+f);
  }
}
