PROTO_OUT = ./src/protocol
PROTO = $(PROTO_OUT)/src
PROTO_PY = $(wildcard src/protocol/prototype/*.py)
PROTO_SRC = $(patsubst %.py, $(PROTO)/%.c, $(PROTO_PY))

$(PROTO)/%.c: %.py
	$(PY) $< $(PROTO_OUT)

$(PROTO): $(PROTO_SRC)