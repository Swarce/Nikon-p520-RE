import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.*;
import java.io.*;
import java.util.*;
public class ApplyAll extends GhidraScript {
  static boolean dataName(String n){
    return n.matches("^(g_|s_|gs|gUI|g[A-Z]|uc|us|ul|ui|pst|pc|st[A-Z]|sz|a[A-Z]|b[A-Z]|by|dw|w[A-Z]|e[A-Z]|tbl|Tbl|TBL|DAT_|.*::__FUNCTION__|.*_Table|.*Tbl$|.*TABLE.*).*");
  }
  public void run() throws Exception {
    String path=getScriptArgs()[0];
    BufferedReader br=new BufferedReader(new FileReader(path));
    String l; int n=0,f=0;
    List<long[]> fn=new ArrayList<>(); List<String> fnn=new ArrayList<>();
    Memory mem=currentProgram.getMemory();
    for(MemoryBlock b: mem.getBlocks()){ b.setWrite(true); b.setExecute(true);}
    while((l=br.readLine())!=null){
      String[] p=l.trim().split("\\s+",3); if(p.length<3) continue;
      long a=Long.parseLong(p[0],16);
      if(a<0xc0000000L||a>=0xc1640000L) continue;
      Address ad=toAddr(a);
      String nm=p[2].replaceAll("[\\s]","_");
      try{ createLabel(ad,nm,true,SourceType.IMPORTED); n++; }catch(Exception e){}
      if((p[1].equals("T")||p[1].equals("t")||p[1].equals("W")) && (a&3)==0 && !dataName(nm)){ fn.add(new long[]{a}); fnn.add(nm);}
    }
    println("labels "+n+", candidate funcs "+fn.size());
    for(int i=0;i<fn.size();i++){
      Address ad=toAddr(fn.get(i)[0]);
      if(getFunctionAt(ad)!=null) continue;
      try{
        disassemble(ad);
        if(getInstructionAt(ad)!=null){ createFunction(ad,fnn.get(i)); f++; }
      }catch(Exception e){}
      if(i%5000==0) println("progress "+i+" funcs "+f);
    }
    println("funcs "+f);
  }
}
