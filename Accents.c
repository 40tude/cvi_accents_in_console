#include <windows.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
typedef DWORD (WINAPI *LPGetNumberOfConsoleFonts)(void);

// ----------------------------------------------------------------------------
int main () {
  
  LPGetNumberOfConsoleFonts   GetNumberOfConsoleFonts;
  HMODULE                     hmod;
  HANDLE                      OutConsole;
  static CONSOLE_FONT_INFOEX  font;
  static CONSOLE_FONT_INFOEX  newfont;
  unsigned int                OldCode=0;
  // Permet, en mode debug, dans la fen�tre Variables and Call Stack de CVI, de v�rifier qu'en hexa les '�' sont bine cod�s 'xE9' ce qui en codepage 850 correspond � '�'
  char                        str[] = "Bel �t� et je d�pense mes �";

  // �vite un warning � la compile
  str[0]=str[0];
  
  // ----------------------------------------------------------------------------
  // http://blogs.msdn.com/b/michkap/archive/2011/09/22/10215125.aspx
  OutConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  
  // ATTENTION : penser � bien initialiser lea taille de la structure
  font.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(OutConsole, 0, &font);
  
  if ((font.FontFamily & TMPF_TRUETYPE) == TMPF_TRUETYPE) {
    puts("La console utilise une police TrueType.");
  }else{
    puts("La console n'utilise pas une police TrueType.");
  }
  
  // GetNumberOfConsoleFonts() is a Win SDK undocumented function
  hmod = GetModuleHandleA("KERNEL32.DLL");
  GetNumberOfConsoleFonts = (LPGetNumberOfConsoleFonts)GetProcAddress(hmod, "GetNumberOfConsoleFonts");   
  if (!hmod || !GetNumberOfConsoleFonts){
    FreeLibrary(hmod);
    puts("Y a un probl�me!"); 
    puts("Strike ENTER to exit :");
    getchar();
  }else{
    printf("Il y a %d polices pour la console\n", GetNumberOfConsoleFonts());
  }
  
  // http://fr.wikipedia.org/wiki/Page_de_code_850
  OldCode = GetConsoleOutputCP();
  printf("Le code page courant est : %d\n", OldCode);
  
  puts("Strike ENTER to continue :");
  getchar();
  
  // ----------------------------------------------------------------------------
  // http://support.microsoft.com/kb/99795/en-us
  // Si on met une font TT type Consolas ou Lucida Console �a passe
  puts("****   Affichage simple : Bel �t�  et je d�pense mes �");  
  
  puts("Strike ENTER to continue :");
  getchar();
  
  // ----------------------------------------------------------------------------
  // � = 130 en dec et 0x82 en hexa
  // il n'y a pas de code pour le caract�re '�' 
  // http://fr.wikipedia.org/wiki/Page_de_code_850
  puts("****   Affichage avec des code hexa : Bel \x82t\x82.");
  puts("ATTENTION : pas de code hexa pour le caract\x8Are \"euro\""); 
  
  puts("Strike ENTER to continue :");
  getchar();
  
  // ----------------------------------------------------------------------------
  // http://fr.wikipedia.org/wiki/Windows-1252    
  puts("On tente de passer en code page 1252");
  // http://support.microsoft.com/kb/99795/en-us
  // SetConsoleOutputCP does not effect the displaying of extended characters of the console font named "Raster Font."
  SetConsoleOutputCP(1252); 
  printf("Le code page courant est : %d\n", GetConsoleOutputCP());   
  
  puts("");    
  puts("****   Affichage en 1252 : Bel �t� et je d�pense mes �");
  puts("ATTENTION : les caract�res accentu�s n'apparaissent correctement qu'avec une police TrueType");  
  
  puts("Strike ENTER to continue :");
  getchar();
  
  // ----------------------------------------------------------------------------
  // http://msdn.microsoft.com/en-us/library/system.console.aspx
  puts("On force la police en mode TrueType");
  
  newfont.cbSize = sizeof(CONSOLE_FONT_INFOEX);     
  newfont.FontFamily = TMPF_TRUETYPE; 
  mbstowcs(newfont.FaceName, "Lucida Console", 100);
  // Faut bien initialiser X et Y
  newfont.dwFontSize.Y = font.dwFontSize.Y;
  newfont.dwFontSize.X = font.dwFontSize.X;
  newfont.FontWeight = font.FontWeight;  
  SetCurrentConsoleFontEx(OutConsole, 0, &newfont);
  
  GetNumberOfConsoleFonts = (LPGetNumberOfConsoleFonts)GetProcAddress(hmod, "GetNumberOfConsoleFonts");  
  printf("Il y a dor�navant %d polices pour la console\n", GetNumberOfConsoleFonts());         
  
  puts("Strike ENTER to continue :");
  getchar();
  
  // ----------------------------------------------------------------------------
  puts("");
  puts("****   Affichage 1252 avec la police modifi�e : Bel �t� et je d�pense mes �");         
  puts("ATTENTION : si au d�part la police n'�tait pas TrueType, l'affichage en 1252 qui n'�tait pas correct l'est dor�navant");
  puts("Pensez � jeter un oeil sur les diff�rents type d'affichage pr�c�dents et � observer comment ils ont �volu�s.");
  puts("");
  
  // ----------------------------------------------------------------------------
  SetConsoleOutputCP(850);  
  puts("****   Affichage en 850 avec la police modifi�e : Bel �t� et je d�pense mes �");  
  
  // ----------------------------------------------------------------------------
  puts("");
  // Rouge
  SetConsoleTextAttribute(OutConsole, FOREGROUND_RED|FOREGROUND_INTENSITY);
  puts("Strike ENTER to exit :");
  getchar();
  
  // ----------------------------------------------------------------------------
  // Hide the console and set the default parameters back 
  //ShowWindow( GetConsoleWindow(), SW_HIDE);   
  SetConsoleTextAttribute(OutConsole, FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
  SetConsoleOutputCP(OldCode);    
  SetCurrentConsoleFontEx(OutConsole, 0, &font);         
  FreeLibrary(hmod);
  
}
