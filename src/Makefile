# バージョン
VERSION = 010

# elf2x68k インストールパス
ifndef ELF2X68K_DIR
	ELF2X68K_DIR=/opt/homebrew/Cellar/elf2x68k/20251124
endif

# デフォルトサフィックスを削除
.SUFFIXES:

# 各種コマンド短縮名
CC = m68k-xelf-gcc
AS = m68k-xelf-as

# HAS to GAS コンバータ
HAS2GAS = python3 has2gas.py

# 実行ファイル名
TARGET_FILE = MU4ON.X

# ドキュメントファイル名
DOCUMENT_FILE = MU4ON.TXT

# 配布パッケージファイル名 
PACKAGE_FILE = ../MU4ON$(VERSION).ZIP

# ヘッダ検索パス
INCLUDE_FLAGS = -I${ELF2X68K_DIR}/lib/gcc/m68k-elf/13.4.0/include \
				-I${ELF2X68K_DIR}/m68k-elf/include 

# コンパイルフラグ
COMMON_FLAGS = -m68000 -O2 -msoft-float -specs=nano.specs $(INCLUDE_FLAGS)
CFLAGS = $(COMMON_FLAGS)

# *.c ソースファイル
C_SRCS = main.c

# *.s ソースファイル
ASM_SRCS =

# *.h ヘッダーファイル
HEADER_SRCS =

# リンク対象のライブラリファイル
LIBS = -ldos -liocs

# 中間ファイル生成用ディレクトリ
INTERMEDIATE_DIR = _build/elf2x68k

# オブジェクトファイル
OBJS =	$(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.c,%.o,$(C_SRCS))) \
	    $(addprefix $(INTERMEDIATE_DIR)/,$(patsubst %.s,%.o,$(ASM_SRCS)))

# デフォルトのターゲット
all : ${INTERMEDIATE_DIR}/$(TARGET_FILE)

# 中間生成物の削除
clean : 
	rm -f $(TARGET_FILE) $(PACKAGE_FILE)
	rm -rf $(INTERMEDIATE_DIR)

# 実行ファイルの生成
${INTERMEDIATE_DIR}/$(TARGET_FILE) : $(OBJS)
	mkdir -p $(INTERMEDIATE_DIR)
	$(CC) $(COMMON_FLAGS) -o ${INTERMEDIATE_DIR}/${TARGET_FILE} $(OBJS) $(LIBS)

# *.c ソースのコンパイル
$(INTERMEDIATE_DIR)/%.o : %.c Makefile $(HEADER_SRCS)
	mkdir -p $(INTERMEDIATE_DIR)
	$(CC) $(CFLAGS) -c $*.c -o $(INTERMEDIATE_DIR)/$*.o

# *.s ソースのアセンブル
$(INTERMEDIATE_DIR)/%.o : %.s Makefile $(HEADER_SRCS)
	mkdir -p $(INTERMEDIATE_DIR)
	$(HAS2GAS) < $*.s > $(INTERMEDIATE_DIR)/$*.gas
	$(AS) -c $(INTERMEDIATE_DIR)/$*.gas -o $(INTERMEDIATE_DIR)/$*.o

# 配布用パッケージ作成
package:
	zip -j ${PACKAGE_FILE} ${INTERMEDIATE_DIR}/${TARGET_FILE} ${DOCUMENT_FILE} 
