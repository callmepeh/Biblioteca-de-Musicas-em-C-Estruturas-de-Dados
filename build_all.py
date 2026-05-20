import os
import sys
import subprocess
import shutil

def check_command(cmd):
    return shutil.which(cmd) is not None

def main():
    print("====== SoundTrees Builder ======")
    suffix = ".exe" if sys.platform == "win32" else ""
    
    # 1. Compilar C backends
    print("\n[1/3] Compilando os motores C...")
    
    # VP
    vp_dir = "VP"
    vp_out = f"programa_vp{suffix}"
    print(f" -> Compilando módulo Árvore Rubro-Negra (VP)...")
    try:
        subprocess.run(
            ["gcc", "main.c", "funcoes.c", "-o", vp_out],
            cwd=vp_dir,
            check=True
        )
        print("    [OK] Módulo VP compilado com sucesso.")
    except Exception as e:
        print(f"    [ERRO] Falha ao compilar módulo VP: {e}")
        sys.exit(1)
        
    # 2-3
    t23_dir = "2-3"
    t23_out = f"programa_2_3{suffix}"
    print(f" -> Compilando módulo Árvore 2-3...")
    try:
        subprocess.run(
            ["gcc", "main.c", "funcoes.c", "-o", t23_out],
            cwd=t23_dir,
            check=True
        )
        print("    [OK] Módulo 2-3 compilado com sucesso.")
    except Exception as e:
        print(f"    [ERRO] Falha ao compilar módulo 2-3: {e}")
        sys.exit(1)

    # 2. Verificar PyInstaller
    print("\n[2/3] Verificando PyInstaller para empacotamento...")
    pyinstaller_installed = check_command("pyinstaller")
    
    if not pyinstaller_installed:
        print(" -> PyInstaller não foi detectado no sistema.")
        print(" -> Para criar um executável único (.exe ou binário), instale o PyInstaller executando:")
        print("    pip install pyinstaller")
        print("\nVocê ainda pode executar a interface diretamente via código-fonte com:")
        print("    python interface.py")
        sys.exit(0)
    else:
        print(" -> PyInstaller detectado.")

    # 3. Empacotar executável único
    print("\n[3/3] Criando executável standalone...")
    
    # Delimitador de caminhos do PyInstaller difere por SO (; no Windows, : no Unix)
    delim = ";" if sys.platform == "win32" else ":"
    
    pyinstaller_cmd = [
        "pyinstaller",
        "--onefile",
        "--windowed",
        "--name", "SoundTrees",
        "--add-binary", f"VP/programa_vp{suffix}{delim}VP",
        "--add-binary", f"2-3/programa_2_3{suffix}{delim}2-3",
        "interface.py"
    ]
    
    print(f" Executando comando: {' '.join(pyinstaller_cmd)}")
    try:
        subprocess.run(pyinstaller_cmd, check=True)
        print("\n==========================================")
        print("[SUCESSO] Build completo!")
        print(f"O executável final foi gerado na pasta: {os.path.abspath('dist')}")
        print("==========================================")
    except Exception as e:
        print(f"\n[ERRO] Falha no empacotamento com PyInstaller: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
