import tkinter as tk
from tkinter import ttk, messagebox
import subprocess
import sys
import os
import re

# Configuração de Cores (Tema Dark Premium)
BG_COLOR = "#1e1e2e"       # Background principal
CARD_BG = "#252538"        # Background de cartões/tabelas
NAV_BG = "#11111b"         # Background da barra lateral
TEXT_COLOR = "#cdd6f4"     # Texto principal (claro)
TEXT_MUTED = "#a6adc8"     # Texto secundário
ACCENT_BLUE = "#89b4fa"    # Azul
ACCENT_GREEN = "#a6e3a1"   # Verde
ACCENT_RED = "#f38ba8"     # Vermelho
ACCENT_YELLOW = "#f9e2af"  # Amarelo
BORDER_COLOR = "#313244"   # Borda
SELECT_BG = "#45475a"      # Cor de item selecionado

class MusicLibraryGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("SoundTrees — Music Database Visualizer")
        self.root.geometry("1100x700")
        self.root.configure(bg=BG_COLOR)
        
        # Estado do Backend C
        self.backend_process = None
        self.backend_type = None  # "VP" ou "23"
        self.backend_path = ""
        
        # Dados selecionados na navegação
        self.selected_artist = None
        self.selected_album = None
        
        # Aplicar estilo ttk global
        self.setup_styles()
        
        # Janela de Seleção de Engine Inicial
        self.show_engine_selection()

    def setup_styles(self):
        style = ttk.Style()
        style.theme_use("clam")
        
        # Configurar cores básicas para componentes TTK
        style.configure(".", bg=BG_COLOR, fg=TEXT_COLOR, font=("Inter", 10))
        
        # Notebook (Abas)
        style.configure("TNotebook", background=BG_COLOR, borderwidth=0)
        style.configure("TNotebook.Tab", background=CARD_BG, foreground=TEXT_MUTED, borderwidth=1, padding=(15, 8), font=("Inter", 10, "bold"))
        style.map("TNotebook.Tab", background=[("selected", BG_COLOR)], foreground=[("selected", ACCENT_BLUE)])
        
        # Treeview (Tabelas)
        style.configure("Treeview", background=CARD_BG, fieldbackground=CARD_BG, foreground=TEXT_COLOR, rowheight=30, borderwidth=0, font=("Inter", 10))
        style.configure("Treeview.Heading", background=NAV_BG, foreground=ACCENT_BLUE, borderwidth=0, padding=8, font=("Inter", 10, "bold"))
        style.map("Treeview", background=[("selected", SELECT_BG)], foreground=[("selected", "#ffffff")])
        
        # Scrollbars
        style.configure("Vertical.TScrollbar", background=CARD_BG, borderwidth=0, arrowsize=12)
        
        # Botões Customizados
        style.configure("Accent.TButton", background=ACCENT_BLUE, foreground=NAV_BG, borderwidth=0, font=("Inter", 10, "bold"), padding=8)
        style.map("Accent.TButton", background=[("active", "#b4befe")])
        
        style.configure("Danger.TButton", background=ACCENT_RED, foreground=NAV_BG, borderwidth=0, font=("Inter", 10, "bold"), padding=8)
        style.map("Danger.TButton", background=[("active", "#f5c2e7")])

    def show_engine_selection(self):
        # Frame de sobreposição para seleção de Engine
        self.engine_window = tk.Toplevel(self.root)
        self.engine_window.title("Escolha o Motor de Banco de Dados")
        self.engine_window.geometry("500x320")
        self.engine_window.resizable(False, False)
        self.engine_window.configure(bg=BG_COLOR)
        self.engine_window.transient(self.root)
        self.engine_window.grab_set()
        
        # Centralizar na tela mãe
        self.engine_window.geometry("+%d+%d" % (self.root.winfo_x() + 300, self.root.winfo_y() + 200))
        
        # Título
        lbl_title = tk.Label(self.engine_window, text="SoundTrees", font=("Outfit", 26, "bold"), bg=BG_COLOR, fg=ACCENT_BLUE)
        lbl_title.pack(pady=(30, 5))
        
        lbl_subtitle = tk.Label(self.engine_window, text="Banco de Dados Musical Auto-Balanceado", font=("Inter", 11), bg=BG_COLOR, fg=TEXT_MUTED)
        lbl_subtitle.pack(pady=(0, 20))
        
        lbl_desc = tk.Label(self.engine_window, text="Selecione a estrutura de dados para rodar o motor em C:", font=("Inter", 10), bg=BG_COLOR, fg=TEXT_COLOR)
        lbl_desc.pack(pady=10)
        
        btn_frame = tk.Frame(self.engine_window, bg=BG_COLOR)
        btn_frame.pack(pady=10)
        
        btn_vp = tk.Button(btn_frame, text="🔴 Árvore Rubro-Negra (VP)", font=("Inter", 11, "bold"), bg=CARD_BG, fg=ACCENT_RED, activebackground=SELECT_BG, activeforeground="#fff", bd=1, relief="solid", highlightcolor=BORDER_COLOR, padx=20, pady=10, command=lambda: self.start_backend("VP"))
        btn_vp.pack(side=tk.LEFT, padx=15)
        
        btn_23 = tk.Button(btn_frame, text="🟢 Árvore 2-3", font=("Inter", 11, "bold"), bg=CARD_BG, fg=ACCENT_GREEN, activebackground=SELECT_BG, activeforeground="#fff", bd=1, relief="solid", highlightcolor=BORDER_COLOR, padx=20, pady=10, command=lambda: self.start_backend("23"))
        btn_23.pack(side=tk.LEFT, padx=15)

    def start_backend(self, engine_type):
        self.backend_type = engine_type
        suffix = ".exe" if sys.platform == "win32" else ""
        binary_name = f"programa_vp{suffix}" if engine_type == "VP" else f"programa_2_3{suffix}"
        dir_name = "VP" if engine_type == "VP" else "2-3"
        
        # Se estiver rodando dentro do pacote do PyInstaller
        if getattr(sys, "frozen", False) and hasattr(sys, "_MEIPASS"):
            self.backend_path = os.path.join(sys._MEIPASS, dir_name, binary_name)
        else:
            self.backend_path = os.path.join(dir_name, binary_name)
            
        # Tentar verificar se o executável existe
        if not os.path.exists(self.backend_path):
            messagebox.showerror(
                "Motor não encontrado", 
                f"O executável '{self.backend_path}' não foi encontrado.\n\nPor favor, compile o projeto executando o script de build correspondente."
            )
            return
            
        try:
            self.backend_process = subprocess.Popen(
                [self.backend_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=0
            )
            # Consumir o menu inicial
            self.read_until_prompt()
            self.engine_window.destroy()
            self.build_main_ui()
            self.load_artists()
        except Exception as e:
            messagebox.showerror("Erro ao iniciar motor C", f"Falha ao iniciar {self.backend_path}:\n{str(e)}")

    def read_until_prompt(self):
        buffer = ""
        while True:
            char = self.backend_process.stdout.read(1)
            if not char:
                break
            buffer += char
            if "Escolha uma opcao: " in buffer or "Escolha uma opção: " in buffer:
                break
        return buffer

    def send_command(self, cmd_input):
        if not self.backend_process:
            return ""
        self.backend_process.stdin.write(cmd_input)
        self.backend_process.stdin.flush()
        return self.read_until_prompt()

    def build_main_ui(self):
        # Grid layout para janela principal
        self.root.columnconfigure(0, weight=0)  # Barra lateral
        self.root.columnconfigure(1, weight=1)  # Conteúdo
        self.root.rowconfigure(0, weight=1)
        
        # 1. Barra Lateral (Navegação/Status)
        sidebar = tk.Frame(self.root, bg=NAV_BG, width=220)
        sidebar.grid(row=0, column=0, sticky="ns")
        sidebar.grid_propagate(False)
        
        lbl_logo = tk.Label(sidebar, text="🎵 SoundTrees", font=("Outfit", 18, "bold"), bg=NAV_BG, fg=ACCENT_BLUE)
        lbl_logo.pack(pady=(25, 5))
        
        engine_name = "Rubro-Negra (VP)" if self.backend_type == "VP" else "Árvore 2-3"
        lbl_badge = tk.Label(sidebar, text=engine_name, font=("Inter", 9, "bold"), bg=SELECT_BG, fg=ACCENT_GREEN if self.backend_type == "23" else ACCENT_RED, padx=10, pady=4)
        lbl_badge.pack(pady=5)
        
        # Divisor
        div = tk.Frame(sidebar, height=1, bg=BORDER_COLOR)
        div.pack(fill=tk.X, padx=15, pady=20)
        
        # Botões de Navegação
        self.btn_artists = tk.Button(sidebar, text="🎤 Artistas & Acervo", font=("Inter", 11, "bold"), bg=NAV_BG, fg=TEXT_COLOR, activebackground=CARD_BG, activeforeground="#fff", bd=0, anchor="w", padx=20, pady=12, command=self.show_artists_view)
        self.btn_artists.pack(fill=tk.X)
        
        self.btn_search = tk.Button(sidebar, text="🔍 Busca & Caminho", font=("Inter", 11, "bold"), bg=NAV_BG, fg=TEXT_COLOR, activebackground=CARD_BG, activeforeground="#fff", bd=0, anchor="w", padx=20, pady=12, command=self.show_search_view)
        self.btn_search.pack(fill=tk.X)
        
        # Rodapé da Barra Lateral
        lbl_footer = tk.Label(sidebar, text="Status: Motor C Ativo", font=("Inter", 8), bg=NAV_BG, fg=TEXT_MUTED)
        lbl_footer.pack(side=tk.BOTTOM, pady=20)
        
        # 2. Área de Conteúdo Central (Notebook com abas invisíveis)
        self.notebook = ttk.Notebook(self.root)
        self.notebook.grid(row=0, column=1, sticky="nsew", padx=20, pady=20)
        
        # View 1: Artistas, Álbuns e Músicas
        self.view_acervo = tk.Frame(self.notebook, bg=BG_COLOR)
        self.notebook.add(self.view_acervo, text="Acervo")
        self.setup_acervo_layout()
        
        # View 2: Busca e Simulação de Caminho
        self.view_search = tk.Frame(self.notebook, bg=BG_COLOR)
        self.notebook.add(self.view_search, text="Busca")
        self.setup_search_layout()
        
        # Ocultar abas padrão do notebook (usamos navegação lateral)
        style = ttk.Style()
        style.layout("TNotebook.Tab", [])

    def show_artists_view(self):
        self.notebook.select(0)
        self.btn_artists.configure(bg=CARD_BG, fg=ACCENT_BLUE)
        self.btn_search.configure(bg=NAV_BG, fg=TEXT_COLOR)

    def show_search_view(self):
        self.notebook.select(1)
        self.btn_search.configure(bg=CARD_BG, fg=ACCENT_BLUE)
        self.btn_artists.configure(bg=NAV_BG, fg=TEXT_COLOR)

    # --- LAYOUT DO ACERVO ---
    def setup_acervo_layout(self):
        # Três colunas: Artistas (30%), Álbuns (30%), Músicas (40%)
        self.view_acervo.columnconfigure(0, weight=3)
        self.view_acervo.columnconfigure(1, weight=3)
        self.view_acervo.columnconfigure(2, weight=4)
        self.view_acervo.rowconfigure(0, weight=1)
        
        # COLUNA 1: ARTISTAS
        col_artists = tk.Frame(self.view_acervo, bg=BG_COLOR)
        col_artists.grid(row=0, column=0, sticky="nsew", padx=(0, 10))
        
        lbl_art = tk.Label(col_artists, text="Artistas", font=("Outfit", 14, "bold"), bg=BG_COLOR, fg=ACCENT_BLUE)
        lbl_art.pack(anchor="w", pady=(0, 10))
        
        # Tabela Artistas
        self.tree_artists = ttk.Treeview(col_artists, columns=("nome", "estilo"), show="headings")
        self.tree_artists.heading("nome", text="Nome")
        self.tree_artists.heading("estilo", text="Estilo")
        self.tree_artists.column("nome", width=120)
        self.tree_artists.column("estilo", width=80)
        self.tree_artists.pack(fill=tk.BOTH, expand=True)
        self.tree_artists.bind("<<TreeviewSelect>>", self.on_artist_select)
        
        # Botões Artistas
        btn_frame_art = tk.Frame(col_artists, bg=BG_COLOR)
        btn_frame_art.pack(fill=tk.X, pady=10)
        
        btn_add_art = ttk.Button(btn_frame_art, text="+ Artista", style="Accent.TButton", command=self.dialog_add_artist)
        btn_add_art.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        btn_del_art = ttk.Button(btn_frame_art, text="- Remover", style="Danger.TButton", command=self.remove_selected_artist)
        btn_del_art.pack(side=tk.RIGHT, fill=tk.X, expand=True, padx=(5, 0))
        
        # COLUNA 2: ÁLBUNS
        col_albums = tk.Frame(self.view_acervo, bg=BG_COLOR)
        col_albums.grid(row=0, column=1, sticky="nsew", padx=10)
        
        self.lbl_alb_title = tk.Label(col_albums, text="Álbuns", font=("Outfit", 14, "bold"), bg=BG_COLOR, fg=ACCENT_BLUE)
        self.lbl_alb_title.pack(anchor="w", pady=(0, 10))
        
        # Tabela Álbuns
        self.tree_albums = ttk.Treeview(col_albums, columns=("titulo", "ano"), show="headings")
        self.tree_albums.heading("titulo", text="Título")
        self.tree_albums.heading("ano", text="Ano")
        self.tree_albums.column("titulo", width=130)
        self.tree_albums.column("ano", width=50)
        self.tree_albums.pack(fill=tk.BOTH, expand=True)
        self.tree_albums.bind("<<TreeviewSelect>>", self.on_album_select)
        
        # Botões Álbuns
        btn_frame_alb = tk.Frame(col_albums, bg=BG_COLOR)
        btn_frame_alb.pack(fill=tk.X, pady=10)
        
        self.btn_add_alb = ttk.Button(btn_frame_alb, text="+ Álbum", style="Accent.TButton", command=self.dialog_add_album, state=tk.DISABLED)
        self.btn_add_alb.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        self.btn_del_alb = ttk.Button(btn_frame_alb, text="- Remover", style="Danger.TButton", command=self.remove_selected_album, state=tk.DISABLED)
        self.btn_del_alb.pack(side=tk.RIGHT, fill=tk.X, expand=True, padx=(5, 0))
        
        # COLUNA 3: MÚSICAS
        col_songs = tk.Frame(self.view_acervo, bg=BG_COLOR)
        col_songs.grid(row=0, column=2, sticky="nsew", padx=(10, 0))
        
        self.lbl_song_title = tk.Label(col_songs, text="Músicas", font=("Outfit", 14, "bold"), bg=BG_COLOR, fg=ACCENT_BLUE)
        self.lbl_song_title.pack(anchor="w", pady=(0, 10))
        
        # Tabela Músicas
        self.tree_songs = ttk.Treeview(col_songs, columns=("titulo", "duracao"), show="headings")
        self.tree_songs.heading("titulo", text="Título da Música")
        self.tree_songs.heading("duracao", text="Duração")
        self.tree_songs.column("titulo", width=180)
        self.tree_songs.column("duracao", width=60)
        self.tree_songs.pack(fill=tk.BOTH, expand=True)
        
        # Botões Músicas
        btn_frame_song = tk.Frame(col_songs, bg=BG_COLOR)
        btn_frame_song.pack(fill=tk.X, pady=10)
        
        self.btn_add_song = ttk.Button(btn_frame_song, text="+ Música", style="Accent.TButton", command=self.dialog_add_song, state=tk.DISABLED)
        self.btn_add_song.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        self.btn_del_song = ttk.Button(btn_frame_song, text="- Remover", style="Danger.TButton", command=self.remove_selected_song, state=tk.DISABLED)
        self.btn_del_song.pack(side=tk.RIGHT, fill=tk.X, expand=True, padx=(5, 0))

    # --- LAYOUT DA BUSCA ---
    def setup_search_layout(self):
        self.view_search.columnconfigure(0, weight=1)
        self.view_search.rowconfigure(0, weight=0)
        self.view_search.rowconfigure(1, weight=1)
        
        # Painel Superior (Formulário de Busca)
        search_panel = tk.Frame(self.view_search, bg=CARD_BG, padx=20, pady=20, highlightbackground=BORDER_COLOR, highlightthickness=1)
        search_panel.grid(row=0, column=0, sticky="ew", pady=(0, 20))
        
        lbl_title = tk.Label(search_panel, text="Pesquisar no Catálogo", font=("Outfit", 14, "bold"), bg=CARD_BG, fg=ACCENT_BLUE)
        lbl_title.grid(row=0, column=0, columnspan=3, sticky="w", pady=(0, 15))
        
        # Inputs
        lbl_type = tk.Label(search_panel, text="Tipo de Busca:", bg=CARD_BG, fg=TEXT_COLOR)
        lbl_type.grid(row=1, column=0, sticky="w", pady=5)
        
        self.search_type_var = tk.StringVar(value="Artista (Mostrar Caminho)")
        search_options = ["Artista (Mostrar Caminho)", "Música de Artista", "Artistas por Estilo"]
        self.combo_search_type = ttk.Combobox(search_panel, textvariable=self.search_type_var, values=search_options, state="readonly", width=25)
        self.combo_search_type.grid(row=1, column=1, sticky="w", padx=10, pady=5)
        self.combo_search_type.bind("<<ComboboxSelected>>", self.on_search_type_change)
        
        # Campo 1 (Nome Artista / Estilo)
        self.lbl_field1 = tk.Label(search_panel, text="Nome do Artista:", bg=CARD_BG, fg=TEXT_COLOR)
        self.lbl_field1.grid(row=2, column=0, sticky="w", pady=5)
        self.entry_field1 = tk.Entry(search_panel, bg=BG_COLOR, fg=TEXT_COLOR, insertbackground=TEXT_COLOR, bd=1, relief="solid", width=30)
        self.entry_field1.grid(row=2, column=1, sticky="w", padx=10, pady=5)
        
        # Campo 2 (Nome da Música)
        self.lbl_field2 = tk.Label(search_panel, text="Nome da Música:", bg=CARD_BG, fg=TEXT_COLOR)
        self.lbl_field2.grid(row=3, column=0, sticky="w", pady=5)
        self.entry_field2 = tk.Entry(search_panel, bg=BG_COLOR, fg=TEXT_COLOR, insertbackground=TEXT_COLOR, bd=1, relief="solid", width=30)
        self.entry_field2.grid(row=3, column=1, sticky="w", padx=10, pady=5)
        
        # Ocultar Campo 2 inicialmente
        self.lbl_field2.grid_remove()
        self.entry_field2.grid_remove()
        
        # Botão de Busca
        btn_run_search = ttk.Button(search_panel, text="🔍 Executar Busca", style="Accent.TButton", command=self.execute_search)
        btn_run_search.grid(row=2, column=2, rowspan=2, padx=20, sticky="nsew")
        
        # Painel Inferior (Resultado e Visualização do Caminho)
        result_panel = tk.Frame(self.view_search, bg=CARD_BG, padx=20, pady=20, highlightbackground=BORDER_COLOR, highlightthickness=1)
        result_panel.grid(row=1, column=0, sticky="nsew")
        result_panel.columnconfigure(0, weight=1)
        result_panel.rowconfigure(1, weight=1)
        
        lbl_res_title = tk.Label(result_panel, text="Caminho e Comparações na Árvore", font=("Outfit", 12, "bold"), bg=CARD_BG, fg=ACCENT_YELLOW)
        lbl_res_title.grid(row=0, column=0, sticky="w", pady=(0, 10))
        
        # Caixa de Texto para o Resultado Visual
        self.text_search_output = tk.Text(result_panel, bg=BG_COLOR, fg=TEXT_COLOR, insertbackground=TEXT_COLOR, font=("Courier", 11), wrap=tk.WORD, bd=0)
        self.text_search_output.grid(row=1, column=0, sticky="nsew")
        
        # Scrollbar
        scroll = ttk.Scrollbar(result_panel, command=self.text_search_output.yview)
        scroll.grid(row=1, column=1, sticky="ns")
        self.text_search_output.configure(yscrollcommand=scroll.set)

    def on_search_type_change(self, event=None):
        search_type = self.search_type_var.get()
        if search_type == "Artista (Mostrar Caminho)":
            self.lbl_field1.configure(text="Nome do Artista:")
            self.lbl_field2.grid_remove()
            self.entry_field2.grid_remove()
        elif search_type == "Música de Artista":
            self.lbl_field1.configure(text="Nome do Artista:")
            self.lbl_field2.grid()
            self.entry_field2.grid()
        elif search_type == "Artistas por Estilo":
            self.lbl_field1.configure(text="Gênero / Estilo:")
            self.lbl_field2.grid_remove()
            self.entry_field2.grid_remove()

    # --- LÓGICA DE DADOS ---
    def load_artists(self):
        # Limpar tabela
        for item in self.tree_artists.get_children():
            self.tree_artists.delete(item)
            
        output = self.send_command("4\n")
        
        # Tratar saída do console
        lines = output.split('\n')
        artists = []
        current = {}
        for line in lines:
            line = line.strip()
            if "Nome:" in line:
                current["nome"] = line.split("Nome:")[1].strip()
            elif "Estilo musical:" in line:
                current["estilo"] = line.split("Estilo musical:")[1].strip()
            elif "Tipo:" in line:
                current["tipo"] = line.split("Tipo:")[1].strip()
            elif "Numero de albuns:" in line or "Número de álbuns:" in line:
                current["albuns"] = line.split(":")[-1].strip()
                artists.append(current)
                current = {}
                
        for art in artists:
            self.tree_artists.insert("", tk.END, values=(art.get("nome", ""), art.get("estilo", "")))
            
        # Resetar estados e tabelas filhas
        self.selected_artist = None
        self.selected_album = None
        self.lbl_alb_title.configure(text="Álbuns")
        self.lbl_song_title.configure(text="Músicas")
        self.btn_add_alb.configure(state=tk.DISABLED)
        self.btn_del_alb.configure(state=tk.DISABLED)
        self.btn_add_song.configure(state=tk.DISABLED)
        self.btn_del_song.configure(state=tk.DISABLED)
        
        for item in self.tree_albums.get_children():
            self.tree_albums.delete(item)
        for item in self.tree_songs.get_children():
            self.tree_songs.delete(item)

    def on_artist_select(self, event=None):
        selected = self.tree_artists.selection()
        if not selected:
            return
        artist_name = self.tree_artists.item(selected[0])["values"][0]
        self.selected_artist = artist_name
        self.lbl_alb_title.configure(text=f"Álbuns de {artist_name}")
        self.btn_add_alb.configure(state=tk.NORMAL)
        self.btn_del_alb.configure(state=tk.DISABLED)
        
        # Carregar álbuns
        for item in self.tree_albums.get_children():
            self.tree_albums.delete(item)
        for item in self.tree_songs.get_children():
            self.tree_songs.delete(item)
            
        # Enviar comando para mostrar álbuns
        output = self.send_command(f"5\n{artist_name}\n")
        
        lines = output.split('\n')
        albums = []
        current = {}
        for line in lines:
            line = line.strip()
            if "Titulo:" in line or "Título:" in line:
                current["titulo"] = line.split(":")[-1].strip()
            elif "Ano de lancamento:" in line or "Ano de lançamento:" in line:
                current["ano"] = line.split(":")[-1].strip()
            elif "Numero de musicas:" in line or "Número de músicas:" in line:
                current["musicas"] = line.split(":")[-1].strip()
                albums.append(current)
                current = {}
                
        for alb in albums:
            self.tree_albums.insert("", tk.END, values=(alb.get("titulo", ""), alb.get("ano", "")))

    def on_album_select(self, event=None):
        selected = self.tree_albums.selection()
        if not selected:
            return
        album_name = self.tree_albums.item(selected[0])["values"][0]
        self.selected_album = album_name
        self.lbl_song_title.configure(text=f"Músicas de {album_name}")
        self.btn_del_alb.configure(state=tk.NORMAL)
        self.btn_add_song.configure(state=tk.NORMAL)
        self.btn_del_song.configure(state=tk.DISABLED)
        
        # Carregar músicas
        for item in self.tree_songs.get_children():
            self.tree_songs.delete(item)
            
        output = self.send_command(f"6\n{self.selected_artist}\n{album_name}\n")
        
        lines = output.split('\n')
        songs = []
        for line in lines:
            line = line.strip()
            if "Titulo:" in line or "Título:" in line:
                # Exemplo: "Titulo: YOU DON'T KNOW ME, Duracao: 4 minutos"
                # Exemplo 2-3: "Título: AAA, Duração: 3 minutos"
                parts = re.split(r'[,;]', line)
                if len(parts) >= 2:
                    titulo = parts[0].split(":")[-1].strip()
                    duracao = parts[1].split(":")[-1].replace("minutos", "").replace("minuto", "").strip() + " min"
                    songs.append({"titulo": titulo, "duracao": duracao})
                    
        for song in songs:
            self.tree_songs.insert("", tk.END, values=(song.get("titulo", ""), song.get("duracao", "")))
            
        self.tree_songs.bind("<<TreeviewSelect>>", lambda e: self.btn_del_song.configure(state=tk.NORMAL))

    # --- DIÁLOGOS DE INSERÇÃO ---
    def dialog_add_artist(self):
        dialog = tk.Toplevel(self.root)
        dialog.title("Cadastrar Artista")
        dialog.geometry("380x300")
        dialog.resizable(False, False)
        dialog.configure(bg=CARD_BG)
        dialog.transient(self.root)
        dialog.grab_set()
        dialog.geometry("+%d+%d" % (self.root.winfo_x() + 350, self.root.winfo_y() + 200))
        
        tk.Label(dialog, text="Novo Artista", font=("Outfit", 14, "bold"), bg=CARD_BG, fg=ACCENT_BLUE).pack(pady=15)
        
        form = tk.Frame(dialog, bg=CARD_BG)
        form.pack(padx=20, fill=tk.BOTH, expand=True)
        
        tk.Label(form, text="Nome:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=0, column=0, sticky="w", pady=5)
        entry_nome = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_nome.grid(row=0, column=1, pady=5)
        entry_nome.focus_set()
        
        tk.Label(form, text="Estilo Musical:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=1, column=0, sticky="w", pady=5)
        entry_estilo = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_estilo.grid(row=1, column=1, pady=5)
        
        tk.Label(form, text="Tipo:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=2, column=0, sticky="w", pady=5)
        tipo_var = tk.StringVar(value="Cantor")
        combo_tipo = ttk.Combobox(form, textvariable=tipo_var, values=["Cantor", "Dupla", "Banda", "Grupo"], state="readonly", width=23)
        combo_tipo.grid(row=2, column=1, pady=5)
        
        def salvar():
            nome = entry_nome.get().strip()
            estilo = entry_estilo.get().strip()
            tipo = ["Cantor", "Dupla", "Banda", "Grupo"].index(tipo_var.get()) + 1
            
            if not nome or not estilo:
                messagebox.showwarning("Campos vazios", "Por favor preencha todos os campos.", parent=dialog)
                return
                
            # Enviar comando ao backend C
            output = self.send_command(f"1\n{nome}\n{estilo}\n{tipo}\n")
            if "sucesso" in output.lower():
                messagebox.showinfo("Sucesso", "Artista cadastrado com sucesso!", parent=dialog)
                dialog.destroy()
                self.load_artists()
            else:
                messagebox.showerror("Erro", f"Erro no backend:\n{output}", parent=dialog)
                
        btn_salvar = ttk.Button(dialog, text="Salvar", style="Accent.TButton", command=salvar)
        btn_salvar.pack(pady=15)

    def dialog_add_album(self):
        if not self.selected_artist:
            return
            
        dialog = tk.Toplevel(self.root)
        dialog.title("Cadastrar Álbum")
        dialog.geometry("380x250")
        dialog.resizable(False, False)
        dialog.configure(bg=CARD_BG)
        dialog.transient(self.root)
        dialog.grab_set()
        dialog.geometry("+%d+%d" % (self.root.winfo_x() + 350, self.root.winfo_y() + 200))
        
        tk.Label(dialog, text=f"Novo Álbum para {self.selected_artist}", font=("Outfit", 12, "bold"), bg=CARD_BG, fg=ACCENT_BLUE, wrap=True).pack(pady=15, px=10)
        
        form = tk.Frame(dialog, bg=CARD_BG)
        form.pack(padx=20, fill=tk.BOTH, expand=True)
        
        tk.Label(form, text="Título:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=0, column=0, sticky="w", pady=5)
        entry_titulo = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_titulo.grid(row=0, column=1, pady=5)
        entry_titulo.focus_set()
        
        tk.Label(form, text="Ano de Lançamento:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=1, column=0, sticky="w", pady=5)
        entry_ano = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_ano.grid(row=1, column=1, pady=5)
        
        def salvar():
            titulo = entry_titulo.get().strip()
            ano = entry_ano.get().strip()
            
            if not titulo or not ano:
                messagebox.showwarning("Campos vazios", "Por favor preencha todos os campos.", parent=dialog)
                return
            if not ano.isdigit():
                messagebox.showwarning("Ano Inválido", "O ano deve conter apenas números.", parent=dialog)
                return
                
            output = self.send_command(f"2\n{self.selected_artist}\n{titulo}\n{ano}\n")
            if "sucesso" in output.lower():
                messagebox.showinfo("Sucesso", "Álbum cadastrado com sucesso!", parent=dialog)
                dialog.destroy()
                self.on_artist_select() # Atualizar álbuns
            else:
                messagebox.showerror("Erro", f"Erro no backend:\n{output}", parent=dialog)
                
        btn_salvar = ttk.Button(dialog, text="Salvar", style="Accent.TButton", command=salvar)
        btn_salvar.pack(pady=15)

    def dialog_add_song(self):
        if not self.selected_artist or not self.selected_album:
            return
            
        dialog = tk.Toplevel(self.root)
        dialog.title("Cadastrar Música")
        dialog.geometry("380x250")
        dialog.resizable(False, False)
        dialog.configure(bg=CARD_BG)
        dialog.transient(self.root)
        dialog.grab_set()
        dialog.geometry("+%d+%d" % (self.root.winfo_x() + 350, self.root.winfo_y() + 200))
        
        tk.Label(dialog, text=f"Nova Música para {self.selected_album}", font=("Outfit", 12, "bold"), bg=CARD_BG, fg=ACCENT_BLUE, wrap=True).pack(pady=15, px=10)
        
        form = tk.Frame(dialog, bg=CARD_BG)
        form.pack(padx=20, fill=tk.BOTH, expand=True)
        
        tk.Label(form, text="Título:", bg=CARD_BG, fg=TEXT_COLOR).grid(row=0, column=0, sticky="w", pady=5)
        entry_titulo = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_titulo.grid(row=0, column=1, pady=5)
        entry_titulo.focus_set()
        
        tk.Label(form, text="Duração (minutos):", bg=CARD_BG, fg=TEXT_COLOR).grid(row=1, column=0, sticky="w", pady=5)
        entry_min = tk.Entry(form, bg=BG_COLOR, fg=TEXT_COLOR, bd=1, relief="solid", width=25)
        entry_min.grid(row=1, column=1, pady=5)
        
        def salvar():
            titulo = entry_titulo.get().strip()
            minutos = entry_min.get().strip()
            
            if not titulo or not minutos:
                messagebox.showwarning("Campos vazios", "Por favor preencha todos os campos.", parent=dialog)
                return
            if not minutos.isdigit():
                messagebox.showwarning("Duração Inválida", "A duração deve ser em minutos inteiros.", parent=dialog)
                return
                
            output = self.send_command(f"3\n{self.selected_artist}\n{self.selected_album}\n{titulo}\n{minutos}\n")
            if "sucesso" in output.lower():
                messagebox.showinfo("Sucesso", "Música cadastrada com sucesso!", parent=dialog)
                dialog.destroy()
                self.on_album_select() # Atualizar músicas
            else:
                messagebox.showerror("Erro", f"Erro no backend:\n{output}", parent=dialog)
                
        btn_salvar = ttk.Button(dialog, text="Salvar", style="Accent.TButton", command=salvar)
        btn_salvar.pack(pady=15)

    # --- REMOÇÃO DE REGISTROS ---
    def remove_selected_artist(self):
        selected = self.tree_artists.selection()
        if not selected:
            return
        artist_name = self.tree_artists.item(selected[0])["values"][0]
        
        confirm = messagebox.askyesno("Confirmar Remoção", f"Deseja realmente remover o artista '{artist_name}' e todos os seus álbuns/músicas?")
        if confirm:
            output = self.send_command(f"14\n{artist_name}\n")
            messagebox.showinfo("Remoção", "Comando de remoção enviado.\n" + output.strip())
            self.load_artists()

    def remove_selected_album(self):
        selected = self.tree_albums.selection()
        if not selected or not self.selected_artist:
            return
        album_name = self.tree_albums.item(selected[0])["values"][0]
        
        confirm = messagebox.askyesno("Confirmar Remoção", f"Deseja realmente remover o álbum '{album_name}' e todas as suas músicas?")
        if confirm:
            output = self.send_command(f"13\n{self.selected_artist}\n{album_name}\n")
            messagebox.showinfo("Remoção", "Comando de remoção enviado.\n" + output.strip())
            self.on_artist_select()

    def remove_selected_song(self):
        selected = self.tree_songs.selection()
        if not selected or not self.selected_artist or not self.selected_album:
            return
        song_name = self.tree_songs.item(selected[0])["values"][0]
        
        confirm = messagebox.askyesno("Confirmar Remoção", f"Deseja realmente remover a música '{song_name}'?")
        if confirm:
            output = self.send_command(f"12\n{self.selected_artist}\n{self.selected_album}\n{song_name}\n")
            messagebox.showinfo("Remoção", "Comando de remoção enviado.\n" + output.strip())
            self.on_album_select()

    # --- BUSCA E SIMULAÇÃO DE CAMINHO ---
    def execute_search(self):
        search_type = self.search_type_var.get()
        field1 = self.entry_field1.get().strip()
        field2 = self.entry_field2.get().strip()
        
        if not field1:
            messagebox.showwarning("Campo vazio", "Preencha o campo de busca.")
            return
            
        self.text_search_output.delete("1.0", tk.END)
        
        if search_type == "Artista (Mostrar Caminho)":
            output = self.send_command(f"8\n{field1}\n")
            
            # Formatando a exibição do caminho de busca
            self.text_search_output.insert(tk.END, "====== BUSCA POR CAMINHO NA ÁRVORE ======\n\n")
            self.text_search_output.insert(tk.END, f"Procurando por: {field1.upper()}\n\n")
            
            # Isolar a parte do caminho
            lines = output.split('\n')
            for line in lines:
                if "caminho" in line.lower() or "-> " in line or "comparacoes" in line.lower() or "comparações" in line.lower():
                    # Colorir nós
                    if "->" in line:
                        nodes = line.split("->")
                        for i, n in enumerate(nodes):
                            if n.strip():
                                if "[ENCONTRADO]" in n:
                                    self.text_search_output.insert(tk.END, f" ➔ {n.strip()}", "success")
                                else:
                                    self.text_search_output.insert(tk.END, f" ➔ {n.strip()}", "node")
                        self.text_search_output.insert(tk.END, "\n")
                    else:
                        self.text_search_output.insert(tk.END, line + "\n")
                        
        elif search_type == "Música de Artista":
            if not field2:
                messagebox.showwarning("Campo vazio", "Preencha o nome da música.")
                return
            output = self.send_command(f"9\n{field1}\n{field2}\n")
            self.text_search_output.insert(tk.END, "====== BUSCA DE MÚSICA POR ARTISTA ======\n\n")
            self.text_search_output.insert(tk.END, f"Artista: {field1.upper()}\nMúsica: {field2.upper()}\n\n")
            self.text_search_output.insert(tk.END, output)
            
        elif search_type == "Artistas por Estilo":
            output = self.send_command(f"11\n{field1}\n")
            self.text_search_output.insert(tk.END, f"====== ARTISTAS DO ESTILO: {field1.upper()} ======\n\n")
            self.text_search_output.insert(tk.END, output)
            
        # Tags de estilo para formatação
        self.text_search_output.tag_configure("node", foreground=ACCENT_BLUE, font=("Courier", 11, "bold"))
        self.text_search_output.tag_configure("success", foreground=ACCENT_GREEN, font=("Courier", 11, "bold"))

    def on_closing(self):
        # Fechar subprocesso C
        if self.backend_process:
            try:
                self.backend_process.stdin.write("0\n")
                self.backend_process.stdin.flush()
                self.backend_process.terminate()
            except:
                pass
        self.root.destroy()

if __name__ == "__main__":
    root = tk.Tk()
    app = MusicLibraryGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()
