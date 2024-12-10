#ifndef PACKETS
#define PACKETS
struct MovePacket {
  float amount;
  char direction;
}__attribute__((packed));

struct TurnPacket {
  float amount;
}__attribute__((packed));

union PacketUnion {
  MovePacket movePacket;
  TurnPacket turnPacket;
}__attribute__((packed));

struct Packet {
  char type;
  PacketUnion u;
}__attribute__((packed));

#define INS_MOVE 1
#define INS_TURN 2
#define INS_START 3


int validateTurn(struct TurnPacket& tp) {
  return !(tp.amount > 1.0 || tp.amount < 0.0);
} 

#define D_X 0
#define D_Y 1
#define D_Z 2


int isValidMoveDirection(char direction) {
  return direction >= D_X && direction <= D_Z;
}

static uint32_t swapEndian32(uint32_t value) {
    return (value >> 24) |
           ((value << 8) & 0x00FF0000) |
           ((value >> 8) & 0x0000FF00) |
           (value << 24);
}

int validateMove(struct MovePacket& mp) {
  mp.amount = swapEndian32((uint32_t) mp.amount);
  if (mp.amount > 1.0 || mp.amount < 0.0) {
    return 1;
  }
  if (!isValidMoveDirection(mp.direction)) {
    Serial.println("Invalid move direction");
    Serial.println(mp.direction);
  }

  return !isValidMoveDirection(mp.direction);
} 


int validatePacket(struct Packet& p) {
  switch (p.type) {
  case INS_MOVE:
    return validateMove(p.u.movePacket);
  case INS_TURN:
    return validateTurn(p.u.turnPacket);
  default:
    Serial.println("INvalid byte");
    Serial.println(p.type);
    return 1;
  }

  //in case of success return 0
  return 0;
}
#endif
