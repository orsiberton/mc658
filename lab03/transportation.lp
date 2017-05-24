\ LP format - for model browsing. Use MPS format to capture full model detail.
Minimize
  12 C0 + 17 C1 + 19 C2 + 25 C3 + 2 C4 + 28 C5
Subject To
 R0: C0 + C1 = 14
 R1: C2 + C3 = 8
 R2: C4 + C5 = 3
 R3: C0 + C2 + C4 = 12
 R4: C1 + C3 + C5 = 13
Bounds
 C0 <= 28
 C1 <= 28
 C2 <= 28
 C3 <= 28
 C4 <= 28
 C5 <= 28
End
