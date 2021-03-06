FixDecoupled::io_hostIn_bits_data = in'32
FixDecoupled::io_hostIn_bits_target = in'3
T0 = eq FixDecoupled::io_hostIn_bits_target 2'3
FixDecoupled::io_hostIn_bits_op = in'2
T1 = eq FixDecoupled::io_hostIn_bits_op 2'2
FixDecoupled::io_hostIn_bits_dst_y = in'5
FixDecoupled::io_hostIn_bits_dst_x = in'5
T2 = cat'5 FixDecoupled::io_hostIn_bits_dst_x FixDecoupled::io_hostIn_bits_dst_y
FixDecoupled::isBroadcast = eq T2 0'10
FixDecoupled::io_tid_y = in'5
T3 = eq FixDecoupled::io_hostIn_bits_dst_y FixDecoupled::io_tid_y
FixDecoupled::io_tid_x = in'5
T4 = eq FixDecoupled::io_hostIn_bits_dst_x FixDecoupled::io_tid_x
T5 = and T4 T3
T6 = or T5 FixDecoupled::isBroadcast
T7 = eq FixDecoupled::num_target_cycles_hiCount 0'16
T8 = eq FixDecoupled::num_target_cycles_loCount 0'16
T9 = and T8 T7
T10 = lt'10 FixDecoupled::current_pc FixDecoupled::num_insts
T11 = lt'10 FixDecoupled::current_pc FixDecoupled::clock_hi_offset
T12 = eq T11 0'1
FixDecoupled::pc_in_clock_hi = and T12 T10
T13 = eq FixDecoupled::num_target_cycles_hiCount 0'16
T14 = and T13 FixDecoupled::pc_in_clock_hi
T15 = eq FixDecoupled::num_target_cycles_loCount 0'16
T16 = and T15 T11
T17 = or T16 T14
T18 = or T17 T9
FixDecoupled::io_hostIn_valid = in'1
T19 = eq FixDecoupled::hostOutRegValid 0'1
FixDecoupled::io_hostOut_ready = in'1
T20 = or FixDecoupled::io_hostOut_ready T19
T21 = and T20 FixDecoupled::io_hostIn_valid
T22 = and T21 T18
T23 = and T22 T6
T24 = and T23 T1
T25 = and T24 T0
T26 = mux T25 FixDecoupled::io_hostIn_bits_data 0'32
T27 = rd'32 1 FixDecoupled::code_sram FixDecoupled::current_pc_sram
T28 = rsh'5 T27 17'5
T29 = eq T28 31'5
T30 = eq T29 0'1
T31 = mux T30 T28 0'5
T32 = cat'5 0'5 T31
FixDecoupled::io_hostIn_bits_addr = in'10
T33 = eq FixDecoupled::io_hostIn_bits_target 0'3
T34 = eq FixDecoupled::io_hostIn_bits_op 1'2
T35 = and T23 T34
T36 = and T35 T33
T37 = mux T36 FixDecoupled::io_hostIn_bits_addr T32
T38 = rsh'5 T37 0'1
FixDecoupled:file::io_reads_0_adr = mov T38
T39 = rd'32 1 FixDecoupled:file::mem FixDecoupled:file::io_reads_0_adr
FixDecoupled:file::io_reads_0_dat = mov T39
FixDecoupled::io_ports_0_in_bits = in'32
FixDecoupled::io_ports_1_in_bits = in'32
T40 = rsh'2 T27 4'3
T41 = rsh'1 T40 0'1
T42 = mux T41 FixDecoupled::io_ports_1_in_bits FixDecoupled::io_ports_0_in_bits
FixDecoupled::io_ports_2_in_bits = in'32
FixDecoupled::io_ports_3_in_bits = in'32
T43 = rsh'1 T40 0'1
T44 = mux T43 FixDecoupled::io_ports_3_in_bits FixDecoupled::io_ports_2_in_bits
T45 = rsh'1 T40 1'1
T46 = mux T45 T44 T42
T47 = mux T29 T46 FixDecoupled:file::io_reads_0_dat
T48 = mux T30 FixDecoupled:file::io_reads_0_dat T47
T49 = rsh'5 T27 6'3
T50 = eq T49 31'5
T51 = rsh'5 T27 27'5
FixDecoupled::isStOp = eq T51 25'5
FixDecoupled::isMuxOp = eq T51 12'5
FixDecoupled::isUseZ = or FixDecoupled::isMuxOp FixDecoupled::isStOp
T52 = eq FixDecoupled::isUseZ 0'1
T53 = or T52 T50
T54 = eq T53 0'1
T55 = mux T54 T49 0'5
FixDecoupled:file::io_reads_2_adr = mov T55
T56 = rd'32 1 FixDecoupled:file::mem FixDecoupled:file::io_reads_2_adr
FixDecoupled:file::io_reads_2_dat = mov T56
T57 = cat'5 0'27 T49
T58 = mux T52 T57 FixDecoupled:file::io_reads_2_dat
T59 = eq T52 0'1
T60 = and T59 T50
T61 = mux T60 T46 T58
T62 = mux T54 FixDecoupled:file::io_reads_2_dat T61
T63 = rsh'1 T62 0'1
T64 = eq 25'5 T51
FixDecoupled::io_ports_0_in_valid = in'1
FixDecoupled::io_ports_1_in_valid = in'1
T65 = rsh'1 T40 0'1
T66 = mux T65 FixDecoupled::io_ports_1_in_valid FixDecoupled::io_ports_0_in_valid
FixDecoupled::io_ports_2_in_valid = in'1
FixDecoupled::io_ports_3_in_valid = in'1
T67 = rsh'1 T40 0'1
T68 = mux T67 FixDecoupled::io_ports_3_in_valid FixDecoupled::io_ports_2_in_valid
T69 = rsh'1 T40 1'1
T70 = mux T69 T68 T66
T71 = eq T49 31'5
T72 = and FixDecoupled::isUseZ T71
FixDecoupled::isStiOp = eq T51 27'5
T73 = eq FixDecoupled::isStiOp 0'1
FixDecoupled::isLitOp = eq T51 2'5
T74 = eq FixDecoupled::isLitOp 0'1
FixDecoupled::isLdiOp = eq T51 26'5
T75 = eq FixDecoupled::isLdiOp 0'1
T76 = rsh'5 T27 11'4
T77 = eq T76 31'5
T78 = rsh'1 T27 16'5
T79 = eq T78 0'1
T80 = and T79 T77
T81 = and T80 T75
T82 = and T81 T74
T83 = and T82 T73
T84 = eq FixDecoupled::isLitOp 0'1
T85 = eq FixDecoupled::isLdiOp 0'1
T86 = eq T28 31'5
T87 = and T86 T85
T88 = and T87 T84
T89 = or T88 T83
T90 = or T89 T72
T91 = rsh'4 T27 0'1
T92 = neq T91 0'4
T93 = rsh'5 T27 22'5
T94 = eq T93 31'5
T95 = eq T94 0'1
T96 = and T95 T92
FixDecoupled::isUseIn = or T96 T90
T97 = eq FixDecoupled::isUseIn 0'1
T98 = or T97 T70
FixDecoupled::io_ports_3_out_ready = in'1
T99 = rsh'1 T91 3'2
T100 = eq T99 0'1
T101 = or T100 FixDecoupled::io_ports_3_out_ready
FixDecoupled::io_ports_2_out_ready = in'1
T102 = rsh'1 T91 2'2
T103 = eq T102 0'1
T104 = or T103 FixDecoupled::io_ports_2_out_ready
FixDecoupled::io_ports_1_out_ready = in'1
T105 = rsh'1 T91 1'1
T106 = eq T105 0'1
T107 = or T106 FixDecoupled::io_ports_1_out_ready
FixDecoupled::io_ports_0_out_ready = in'1
T108 = rsh'1 T91 0'1
T109 = eq T108 0'1
T110 = or T109 FixDecoupled::io_ports_0_out_ready
T111 = and T110 T107
T112 = and T111 T104
T113 = and T112 T101
T114 = and T113 T98
T115 = eq T18 0'1
T116 = and T115 T114
T117 = and T116 T64
T118 = and T117 T63
T119 = mux T118 T48 T26
T120 = eq 27'5 T51
T121 = and T116 T120
T122 = mux T121 T48 T119
FixDecoupled:mem::io_writes_0_dat = mov T122
T123 = mov FixDecoupled:mem::io_writes_0_dat
T124 = mux T25 T25 0'1
T125 = mux T118 T118 T124
T126 = mux T121 T121 T125
FixDecoupled:mem::io_writes_0_is = mov T126
T127 = mux T25 FixDecoupled::io_hostIn_bits_addr 0'10
T128 = cat'10 0'22 T127
T129 = eq T76 31'5
T130 = or T78 T129
T131 = eq T130 0'1
T132 = mux T131 T76 0'5
FixDecoupled:file::io_reads_1_adr = mov T132
T133 = rd'32 1 FixDecoupled:file::mem FixDecoupled:file::io_reads_1_adr
FixDecoupled:file::io_reads_1_dat = mov T133
T134 = cat'5 0'27 T76
T135 = mux T78 T134 FixDecoupled:file::io_reads_1_dat
T136 = eq T78 0'1
T137 = and T136 T129
T138 = mux T137 T46 T135
T139 = mux T131 FixDecoupled:file::io_reads_1_dat T138
T140 = mux T118 T139 T128
T141 = cat'1 T93 T78
T142 = cat'5 T141 T76
FixDecoupled::sti_addr = cat'5 T142 T49
T143 = cat'16 0'16 FixDecoupled::sti_addr
T144 = mux T121 T143 T140
T145 = rsh'10 T144 0'1
FixDecoupled:mem::io_writes_0_adr = mov T145
T146 = wr'32 FixDecoupled:mem::io_writes_0_is FixDecoupled:mem::mem FixDecoupled:mem::io_writes_0_adr T123
FixDecoupled:mem::mem = mem'32 1024
T147 = eq FixDecoupled::io_hostIn_bits_target 0'3
T148 = and T24 T147
T149 = mux T148 FixDecoupled::io_hostIn_bits_data 0'32
T150 = cat'1 T28 T78
T151 = cat'5 T150 T76
FixDecoupled::lit = cat'5 T151 T49
T152 = eq 2'5 T51
T153 = and T116 T152
T154 = mux T153 FixDecoupled::lit 0'16
T155 = cat'1 0'15 FixDecoupled::isReset
T156 = eq 1'5 T51
T157 = and T116 T156
T158 = mux T157 T155 T154
T159 = cat'16 0'16 T158
T160 = not'32 T48
T161 = eq 6'5 T51
T162 = and T116 T161
T163 = mux T162 T160 T159
T164 = and T48 T139
T165 = eq 7'5 T51
T166 = and T116 T165
T167 = mux T166 T164 T163
T168 = or T48 T139
T169 = eq 8'5 T51
T170 = and T116 T169
T171 = mux T170 T168 T167
T172 = xor T48 T139
T173 = eq 9'5 T51
T174 = and T116 T173
T175 = mux T174 T172 T171
T176 = eq T48 T139
T177 = cat'1 0'31 T176
T178 = eq 10'5 T51
T179 = and T116 T178
T180 = mux T179 T177 T175
T181 = eq T48 T139
T182 = eq T181 0'1
T183 = cat'1 0'31 T182
T184 = eq 11'5 T51
T185 = and T116 T184
T186 = mux T185 T183 T180
T187 = rsh'1 T48 0'1
T188 = mux T187 T139 T62
T189 = eq 12'5 T51
T190 = and T116 T189
T191 = mux T190 T188 T186
FixDecoupled::sa = rsh'5 T139 0'1
T192 = lsh'32 T48 FixDecoupled::sa
T193 = rsh'32 T192 0'1
T194 = eq 13'5 T51
T195 = and T116 T194
T196 = mux T195 T193 T191
T197 = rsh'32 T48 FixDecoupled::sa
T198 = eq 14'5 T51
T199 = and T116 T198
T200 = mux T199 T197 T196
T201 = lsh'32 1'1 T49
T202 = sub'32 T201 1'32
T203 = rsh'32 T202 0'1
T204 = eq T49 0'5
FixDecoupled::mask = mux T204 4294967295'32 T203
T205 = and T139 FixDecoupled::mask
T206 = lsh'32 T48 T49
T207 = rsh'32 T206 0'1
T208 = or T207 T205
T209 = eq 17'5 T51
T210 = and T116 T209
T211 = mux T210 T208 T200
T212 = add'32 T48 T139
T213 = eq 18'5 T51
T214 = and T116 T213
T215 = mux T214 T212 T211
T216 = sub'32 T48 T139
T217 = eq 19'5 T51
T218 = and T116 T217
T219 = mux T218 T216 T215
T220 = lt'32 T48 T139
T221 = cat'1 0'31 T220
T222 = eq 20'5 T51
T223 = and T116 T222
T224 = mux T223 T221 T219
T225 = gte'32 T48 T139
T226 = cat'1 0'31 T225
T227 = eq 21'5 T51
T228 = and T116 T227
T229 = mux T228 T226 T224
T230 = rsh'16 T139 0'1
T231 = rsh'16 T48 0'1
T232 = mul'32 T231 T230
T233 = eq 22'5 T51
T234 = and T116 T233
T235 = mux T234 T232 T229
T236 = log2'5 T48
T237 = cat'5 0'27 T236
T238 = eq 23'5 T51
T239 = and T116 T238
T240 = mux T239 T237 T235
T241 = eq FixDecoupled::io_hostIn_bits_target 2'3
T242 = and T35 T241
T243 = mux T242 FixDecoupled::io_hostIn_bits_addr 0'10
T244 = cat'10 0'22 T243
T245 = rsh'1 T139 0'1
T246 = eq 24'5 T51
T247 = and T116 T246
T248 = and T247 T245
T249 = mux T248 T48 T244
T250 = cat'1 T28 T78
T251 = cat'5 T250 T76
FixDecoupled::ldi_addr = cat'5 T251 T49
T252 = cat'16 0'16 FixDecoupled::ldi_addr
T253 = eq 26'5 T51
T254 = and T116 T253
T255 = mux T254 T252 T249
T256 = rsh'10 T255 0'1
FixDecoupled:mem::io_reads_0_adr = mov T256
T257 = rd'32 1 FixDecoupled:mem::mem FixDecoupled:mem::io_reads_0_adr
FixDecoupled:mem::io_reads_0_dat = mov T257
T258 = mux T248 FixDecoupled:mem::io_reads_0_dat T240
T259 = mux T254 FixDecoupled:mem::io_reads_0_dat T258
T260 = and T259 FixDecoupled::mask
T261 = mux T166 0'1 T162
T262 = mux T170 0'1 T261
T263 = mux T174 0'1 T262
T264 = mux T179 0'1 T263
T265 = mux T185 0'1 T264
T266 = mux T190 0'1 T265
T267 = mux T195 0'1 T266
T268 = mux T199 1'1 T267
T269 = mux T210 0'1 T268
T270 = mux T214 1'1 T269
T271 = mux T218 1'1 T270
T272 = mux T223 0'1 T271
T273 = mux T228 0'1 T272
T274 = mux T234 1'1 T273
T275 = mux T239 0'1 T274
T276 = mux T248 0'1 T275
T277 = mux T254 0'1 T276
T278 = mux T277 T260 T259
T279 = mux T153 T93 0'5
T280 = mux T157 T93 T279
T281 = mux T162 T93 T280
T282 = mux T166 T93 T281
T283 = mux T170 T93 T282
T284 = mux T174 T93 T283
T285 = mux T179 T93 T284
T286 = mux T185 T93 T285
T287 = mux T190 T93 T286
T288 = mux T195 T93 T287
T289 = mux T199 T93 T288
T290 = mux T210 T93 T289
T291 = mux T214 T93 T290
T292 = mux T218 T93 T291
T293 = mux T223 T93 T292
T294 = mux T228 T93 T293
T295 = mux T234 T93 T294
T296 = mux T239 T93 T295
T297 = mux T248 T93 T296
T298 = mux T254 T93 T297
T299 = eq T298 31'5
T300 = eq T299 0'1
T301 = mux T157 1'1 T153
T302 = mux T162 1'1 T301
T303 = mux T166 1'1 T302
T304 = mux T170 1'1 T303
T305 = mux T174 1'1 T304
T306 = mux T179 1'1 T305
T307 = mux T185 1'1 T306
T308 = mux T190 1'1 T307
T309 = mux T195 1'1 T308
T310 = mux T199 1'1 T309
T311 = mux T210 1'1 T310
T312 = mux T214 1'1 T311
T313 = mux T218 1'1 T312
T314 = mux T223 1'1 T313
T315 = mux T228 1'1 T314
T316 = mux T234 1'1 T315
T317 = mux T239 1'1 T316
T318 = mux T248 1'1 T317
T319 = mux T254 1'1 T318
T320 = and T116 T319
T321 = and T320 T300
T322 = mux T321 T278 T149
FixDecoupled:file::io_writes_0_dat = mov T322
T323 = mov FixDecoupled:file::io_writes_0_dat
T324 = mux T148 FixDecoupled::io_hostIn_bits_addr 0'10
T325 = cat'5 0'5 T298
T326 = mux T321 T325 T324
T327 = rsh'5 T326 0'1
FixDecoupled:file::io_writes_0_adr = mov T327
T328 = lt'5 FixDecoupled:file::io_writes_0_adr 31'5
T329 = mux T148 T148 0'1
T330 = mux T321 T321 T329
FixDecoupled:file::io_writes_0_is = mov T330
T331 = and FixDecoupled:file::io_writes_0_is T328
T332 = wr'32 T331 FixDecoupled:file::mem FixDecoupled:file::io_writes_0_adr T323
FixDecoupled:file::mem = mem'32 31
FixDecoupled::io_hostIn_ready = out'1 T20
FixDecoupled::io_hostOut_valid = out'1 FixDecoupled::hostOutRegValid
T333 = mux T21 FixDecoupled::io_hostIn_bits_op FixDecoupled::hostOutReg_op
FixDecoupled::hostOutReg_op = reg'2 1 T333
FixDecoupled::io_hostOut_bits_op = out'2 FixDecoupled::hostOutReg_op
T334 = mux T21 FixDecoupled::io_hostIn_bits_dst_x FixDecoupled::hostOutReg_dst_x
FixDecoupled::hostOutReg_dst_x = reg'5 1 T334
FixDecoupled::io_hostOut_bits_dst_x = out'5 FixDecoupled::hostOutReg_dst_x
T335 = mux T21 FixDecoupled::io_hostIn_bits_dst_y FixDecoupled::hostOutReg_dst_y
FixDecoupled::hostOutReg_dst_y = reg'5 1 T335
FixDecoupled::io_hostOut_bits_dst_y = out'5 FixDecoupled::hostOutReg_dst_y
T336 = mux T21 FixDecoupled::io_hostIn_bits_target FixDecoupled::hostOutReg_target
FixDecoupled::hostOutReg_target = reg'3 1 T336
FixDecoupled::io_hostOut_bits_target = out'3 FixDecoupled::hostOutReg_target
T337 = mux T21 FixDecoupled::io_hostIn_bits_addr FixDecoupled::hostOutReg_addr
FixDecoupled::hostOutReg_addr = reg'10 1 T337
FixDecoupled::io_hostOut_bits_addr = out'10 FixDecoupled::hostOutReg_addr
reset = rst
T338 = eq FixDecoupled::io_hostIn_bits_target 3'3
T339 = and T24 T338
T340 = mux T339 0'32 FixDecoupled::num_host_cycles
T341 = lt'10 0'10 FixDecoupled::num_insts
T342 = eq FixDecoupled::io_hostIn_bits_target 5'3
T343 = and T24 T342
T344 = and T343 T341
T345 = mux T344 0'32 T340
T346 = eq FixDecoupled::io_hostIn_bits_target 6'3
T347 = and T24 T346
T348 = mux T347 FixDecoupled::io_hostIn_bits_data T345
T349 = add'32 FixDecoupled::num_host_cycles 1'32
T350 = mux T115 T349 T348
FixDecoupled::num_host_cycles__update = mux'32 reset 0'32 T350
FixDecoupled::num_host_cycles = reg'32 1 FixDecoupled::num_host_cycles__update
T351 = mux T21 FixDecoupled::io_hostIn_bits_data FixDecoupled::hostOutReg_data
T352 = cat'16 FixDecoupled::num_target_cycles_loCount FixDecoupled::num_target_cycles_hiCount
T353 = eq FixDecoupled::io_hostIn_bits_target 5'3
T354 = eq FixDecoupled::io_hostIn_bits_op 1'2
T355 = eq FixDecoupled::io_hostIn_bits_data 0'32
T356 = and FixDecoupled::isBroadcast T355
T357 = and T356 T354
T358 = and T357 T353
T359 = and T21 T358
T360 = mux T359 T352 T351
T361 = mux T36 FixDecoupled:file::io_reads_0_dat T360
T362 = mux T242 FixDecoupled:mem::io_reads_0_dat T361
T363 = cat'1 0'31 FixDecoupled::isReset
T364 = eq FixDecoupled::io_hostIn_bits_target 3'3
T365 = and T35 T364
T366 = mux T365 T363 T362
T367 = cat'10 0'22 FixDecoupled::num_insts
T368 = eq FixDecoupled::io_hostIn_bits_target 4'3
T369 = and T35 T368
T370 = mux T369 T367 T366
T371 = cat'10 0'22 FixDecoupled::clock_hi_offset
T372 = eq FixDecoupled::io_hostIn_bits_target 7'3
T373 = and T35 T372
T374 = mux T373 T371 T370
T375 = eq FixDecoupled::isBroadcast 0'1
T376 = eq FixDecoupled::io_hostIn_bits_target 5'3
T377 = and T35 T376
T378 = and T377 T375
T379 = mux T378 T352 T374
T380 = lt'32 FixDecoupled::io_hostIn_bits_data FixDecoupled::num_host_cycles
T381 = eq FixDecoupled::io_hostIn_bits_target 6'3
T382 = and T35 T381
T383 = and T382 T380
T384 = mux T383 FixDecoupled::num_host_cycles T379
FixDecoupled::hostOutReg_data = reg'32 1 T384
FixDecoupled::io_hostOut_bits_data = out'32 FixDecoupled::hostOutReg_data
T385 = rsh'1 T91 0'1
T386 = and T116 T385
FixDecoupled::io_ports_0_out_valid = out'1 T386
T387 = and T320 T299
T388 = mux T387 T259 T46
T389 = mux T386 T388 0'32
FixDecoupled::io_ports_0_out_bits = out'32 T389
T390 = lsh'7 1'4 T40
T391 = rsh'4 T390 0'1
T392 = rsh'1 T391 0'1
T393 = and T116 FixDecoupled::isUseIn
T394 = and T393 T392
FixDecoupled::io_ports_0_in_ready = out'1 T394
T395 = rsh'1 T91 1'1
T396 = and T116 T395
FixDecoupled::io_ports_1_out_valid = out'1 T396
T397 = mux T396 T388 0'32
FixDecoupled::io_ports_1_out_bits = out'32 T397
T398 = rsh'1 T391 1'1
T399 = and T393 T398
FixDecoupled::io_ports_1_in_ready = out'1 T399
T400 = rsh'1 T91 2'2
T401 = and T116 T400
FixDecoupled::io_ports_2_out_valid = out'1 T401
T402 = mux T401 T388 0'32
FixDecoupled::io_ports_2_out_bits = out'32 T402
T403 = rsh'1 T391 2'2
T404 = and T393 T403
FixDecoupled::io_ports_2_in_ready = out'1 T404
T405 = rsh'1 T91 3'2
T406 = and T116 T405
FixDecoupled::io_ports_3_out_valid = out'1 T406
T407 = rsh'1 FixDecoupled::io_hostIn_bits_data 0'1
T408 = mux T339 T407 FixDecoupled::isReset
T409 = mux T157 0'1 T408
FixDecoupled::isReset__update = mux'1 reset 0'1 T409
FixDecoupled::isReset = reg'1 1 FixDecoupled::isReset__update
T410 = mux T406 T388 0'32
FixDecoupled::io_ports_3_out_bits = out'32 T410
T411 = eq T114 0'1
T412 = eq T18 0'1
T413 = and T412 T411
T414 = mux T413 FixDecoupled::current_pc 0'10
FixDecoupled::inc_pc = add'10 FixDecoupled::current_pc 1'10
T415 = gte'10 FixDecoupled::inc_pc FixDecoupled::num_insts
T416 = lt'10 FixDecoupled::inc_pc FixDecoupled::clock_hi_offset
T417 = or T416 T415
T418 = eq FixDecoupled::num_target_cycles_loCount 0'16
T419 = eq FixDecoupled::num_target_cycles_hiCount 0'16
T420 = eq T419 0'1
T421 = and T420 T418
FixDecoupled::goto_hi_offset = and T421 T417
T422 = eq T413 0'1
T423 = and T422 FixDecoupled::goto_hi_offset
T424 = mux T423 FixDecoupled::clock_hi_offset T414
T425 = eq FixDecoupled::num_target_cycles_hiCount 0'16
T426 = eq T425 0'1
T427 = lt'10 FixDecoupled::inc_pc FixDecoupled::num_insts
T428 = gte'10 FixDecoupled::inc_pc FixDecoupled::clock_hi_offset
T429 = and T428 T427
T430 = and T429 T426
T431 = eq FixDecoupled::num_target_cycles_loCount 0'16
T432 = eq T431 0'1
T433 = lt'10 FixDecoupled::inc_pc FixDecoupled::clock_hi_offset
T434 = and T433 T432
FixDecoupled::goto_inc_pc = or T434 T430
T435 = eq FixDecoupled::goto_hi_offset 0'1
T436 = and T435 FixDecoupled::goto_inc_pc
T437 = and T422 T436
T438 = mux T437 FixDecoupled::inc_pc T424
T439 = or FixDecoupled::goto_hi_offset FixDecoupled::goto_inc_pc
T440 = eq T439 0'1
T441 = and T422 T440
T442 = mux T441 0'10 T438
FixDecoupled::current_pc_sram = reg'10 1 T442
T443 = eq FixDecoupled::io_hostOut_ready 0'1
T444 = and FixDecoupled::hostOutRegValid T443
T445 = or FixDecoupled::io_hostIn_valid T444
FixDecoupled::hostOutRegValid__update = mux'1 reset 0'1 T445
FixDecoupled::hostOutRegValid = reg'1 1 FixDecoupled::hostOutRegValid__update
T446 = rsh'16 FixDecoupled::io_hostIn_bits_data 16'5
T447 = mux T344 T446 FixDecoupled::num_target_cycles_loCount
T448 = sub'16 FixDecoupled::num_target_cycles_loCount 1'16
T449 = lt'16 0'16 FixDecoupled::num_target_cycles_loCount
T450 = eq FixDecoupled::inc_pc FixDecoupled::clock_hi_offset
T451 = and T450 T449
T452 = and T116 T451
T453 = mux T452 T448 T447
FixDecoupled::num_target_cycles_loCount__update = mux'16 reset 0'16 T453
FixDecoupled::num_target_cycles_loCount = reg'16 1 FixDecoupled::num_target_cycles_loCount__update
T454 = cat'10 0'22 FixDecoupled::clock_hi_offset
T455 = eq FixDecoupled::io_hostIn_bits_target 7'3
T456 = and T24 T455
T457 = mux T456 FixDecoupled::io_hostIn_bits_data T454
T458 = rsh'10 T457 0'1
FixDecoupled::clock_hi_offset__update = mux'10 reset 0'10 T458
FixDecoupled::clock_hi_offset = reg'10 1 FixDecoupled::clock_hi_offset__update
FixDecoupled::current_pc__update = mux'10 reset 0'10 T442
FixDecoupled::current_pc = reg'10 1 FixDecoupled::current_pc__update
T459 = cat'10 0'22 FixDecoupled::num_insts
T460 = eq FixDecoupled::io_hostIn_bits_target 4'3
T461 = and T24 T460
T462 = mux T461 FixDecoupled::io_hostIn_bits_data T459
T463 = rsh'10 T462 0'1
FixDecoupled::num_insts__update = mux'10 reset 0'10 T463
FixDecoupled::num_insts = reg'10 1 FixDecoupled::num_insts__update
T464 = rsh'16 FixDecoupled::io_hostIn_bits_data 0'1
T465 = mux T344 T464 FixDecoupled::num_target_cycles_hiCount
T466 = sub'16 FixDecoupled::num_target_cycles_hiCount 1'16
T467 = lt'16 0'16 FixDecoupled::num_target_cycles_hiCount
T468 = eq FixDecoupled::inc_pc FixDecoupled::num_insts
T469 = and T468 T467
T470 = and T116 T469
T471 = mux T470 T466 T465
FixDecoupled::num_target_cycles_hiCount__update = mux'16 reset 0'16 T471
FixDecoupled::num_target_cycles_hiCount = reg'16 1 FixDecoupled::num_target_cycles_hiCount__update
T472 = mov FixDecoupled::io_hostIn_bits_data
T473 = eq FixDecoupled::io_hostIn_bits_target 1'3
T474 = and T24 T473
FixDecoupled::code_wen = mov T474
T475 = wr'32 FixDecoupled::code_wen FixDecoupled::code_sram FixDecoupled::io_hostIn_bits_addr T472
FixDecoupled::code_sram = mem'32 1024
T476 = rsh'1 T391 3'2
T477 = and T393 T476
FixDecoupled::io_ports_3_in_ready = out'1 T477
