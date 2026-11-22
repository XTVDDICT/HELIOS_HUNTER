#include "HunterTimezones.h"
#include <string.h>
// Moment Timezone / IANA tzdb 2026f. See TIMEZONE-LICENSE.txt.
// Transition coverage: 2026-01-01 through 2100-12-31.
const HunterZoneTransition HUNTER_ZONE_TRANSITIONS[] = {
  {1767225600U, 0}, // transition 0
  {1767225600U, 10800}, // transition 1
  {1767225600U, 3600}, // transition 2
  {1767225600U, 7200}, // transition 3
  {1767225600U, 7200}, // transition 4
  {1776981600U, 10800}, // transition 5
  {1793307600U, 7200}, // transition 6
  {1809036000U, 10800}, // transition 7
  {1824757200U, 7200}, // transition 8
  {1840485600U, 10800}, // transition 9
  {1856206800U, 7200}, // transition 10
  {1871935200U, 10800}, // transition 11
  {1887656400U, 7200}, // transition 12
  {1903384800U, 10800}, // transition 13
  {1919710800U, 7200}, // transition 14
  {1934834400U, 10800}, // transition 15
  {1951160400U, 7200}, // transition 16
  {1966888800U, 10800}, // transition 17
  {1982610000U, 7200}, // transition 18
  {1998338400U, 10800}, // transition 19
  {2014059600U, 7200}, // transition 20
  {2029788000U, 10800}, // transition 21
  {2045509200U, 7200}, // transition 22
  {2061237600U, 10800}, // transition 23
  {2076958800U, 7200}, // transition 24
  {2092687200U, 10800}, // transition 25
  {2109013200U, 7200}, // transition 26
  {2124136800U, 10800}, // transition 27
  {2140462800U, 7200}, // transition 28
  {2156191200U, 10800}, // transition 29
  {2171912400U, 7200}, // transition 30
  {2187640800U, 10800}, // transition 31
  {2203362000U, 7200}, // transition 32
  {2219090400U, 10800}, // transition 33
  {2234811600U, 7200}, // transition 34
  {2250540000U, 10800}, // transition 35
  {2266866000U, 7200}, // transition 36
  {2281989600U, 10800}, // transition 37
  {2298315600U, 7200}, // transition 38
  {2313439200U, 10800}, // transition 39
  {2329765200U, 7200}, // transition 40
  {2345493600U, 10800}, // transition 41
  {2361214800U, 7200}, // transition 42
  {2376943200U, 10800}, // transition 43
  {2392664400U, 7200}, // transition 44
  {2408392800U, 10800}, // transition 45
  {2424114000U, 7200}, // transition 46
  {2439842400U, 10800}, // transition 47
  {2456168400U, 7200}, // transition 48
  {2471292000U, 10800}, // transition 49
  {2487618000U, 7200}, // transition 50
  {2503346400U, 10800}, // transition 51
  {2519067600U, 7200}, // transition 52
  {2534796000U, 10800}, // transition 53
  {2550517200U, 7200}, // transition 54
  {2566245600U, 10800}, // transition 55
  {2581966800U, 7200}, // transition 56
  {2597695200U, 10800}, // transition 57
  {2614021200U, 7200}, // transition 58
  {2629144800U, 10800}, // transition 59
  {2645470800U, 7200}, // transition 60
  {2660594400U, 10800}, // transition 61
  {2676920400U, 7200}, // transition 62
  {2692648800U, 10800}, // transition 63
  {2708370000U, 7200}, // transition 64
  {2724098400U, 10800}, // transition 65
  {2739819600U, 7200}, // transition 66
  {2755548000U, 10800}, // transition 67
  {2771269200U, 7200}, // transition 68
  {2786997600U, 10800}, // transition 69
  {2803323600U, 7200}, // transition 70
  {2818447200U, 10800}, // transition 71
  {2834773200U, 7200}, // transition 72
  {2850501600U, 10800}, // transition 73
  {2866222800U, 7200}, // transition 74
  {2881951200U, 10800}, // transition 75
  {2897672400U, 7200}, // transition 76
  {2913400800U, 10800}, // transition 77
  {2929122000U, 7200}, // transition 78
  {2944850400U, 10800}, // transition 79
  {2960571600U, 7200}, // transition 80
  {2976300000U, 10800}, // transition 81
  {2992626000U, 7200}, // transition 82
  {3007749600U, 10800}, // transition 83
  {3024075600U, 7200}, // transition 84
  {3039804000U, 10800}, // transition 85
  {3055525200U, 7200}, // transition 86
  {3071253600U, 10800}, // transition 87
  {3086974800U, 7200}, // transition 88
  {3102703200U, 10800}, // transition 89
  {3118424400U, 7200}, // transition 90
  {3134152800U, 10800}, // transition 91
  {3150478800U, 7200}, // transition 92
  {3165602400U, 10800}, // transition 93
  {3181928400U, 7200}, // transition 94
  {3197052000U, 10800}, // transition 95
  {3213378000U, 7200}, // transition 96
  {3229106400U, 10800}, // transition 97
  {3244827600U, 7200}, // transition 98
  {3260556000U, 10800}, // transition 99
  {3276277200U, 7200}, // transition 100
  {3292005600U, 10800}, // transition 101
  {3307726800U, 7200}, // transition 102
  {3323455200U, 10800}, // transition 103
  {3339781200U, 7200}, // transition 104
  {3354904800U, 10800}, // transition 105
  {3371230800U, 7200}, // transition 106
  {3386959200U, 10800}, // transition 107
  {3402680400U, 7200}, // transition 108
  {3418408800U, 10800}, // transition 109
  {3434130000U, 7200}, // transition 110
  {3449858400U, 10800}, // transition 111
  {3465579600U, 7200}, // transition 112
  {3481308000U, 10800}, // transition 113
  {3497634000U, 7200}, // transition 114
  {3512757600U, 10800}, // transition 115
  {3529083600U, 7200}, // transition 116
  {3544207200U, 10800}, // transition 117
  {3560533200U, 7200}, // transition 118
  {3576261600U, 10800}, // transition 119
  {3591982800U, 7200}, // transition 120
  {3607711200U, 10800}, // transition 121
  {3623432400U, 7200}, // transition 122
  {3639160800U, 10800}, // transition 123
  {3654882000U, 7200}, // transition 124
  {3670610400U, 10800}, // transition 125
  {3686936400U, 7200}, // transition 126
  {3702060000U, 10800}, // transition 127
  {3718386000U, 7200}, // transition 128
  {3734114400U, 10800}, // transition 129
  {3749835600U, 7200}, // transition 130
  {3765564000U, 10800}, // transition 131
  {3781285200U, 7200}, // transition 132
  {3797013600U, 10800}, // transition 133
  {3812734800U, 7200}, // transition 134
  {3828463200U, 10800}, // transition 135
  {3844184400U, 7200}, // transition 136
  {3859912800U, 10800}, // transition 137
  {3876238800U, 7200}, // transition 138
  {3891362400U, 10800}, // transition 139
  {3907688400U, 7200}, // transition 140
  {3923416800U, 10800}, // transition 141
  {3939138000U, 7200}, // transition 142
  {3954866400U, 10800}, // transition 143
  {3970587600U, 7200}, // transition 144
  {3986316000U, 10800}, // transition 145
  {4002037200U, 7200}, // transition 146
  {4017765600U, 10800}, // transition 147
  {4034091600U, 7200}, // transition 148
  {4049215200U, 10800}, // transition 149
  {4065541200U, 7200}, // transition 150
  {4080664800U, 10800}, // transition 151
  {4096990800U, 7200}, // transition 152
  {4112719200U, 10800}, // transition 153
  {4128440400U, 7200}, // transition 154
  {1767225600U, 3600}, // transition 155
  {1771120800U, 0}, // transition 156
  {1774144800U, 3600}, // transition 157
  {1789866000U, 0}, // transition 158
  {1767225600U, 3600}, // transition 159
  {1774746000U, 7200}, // transition 160
  {1792890000U, 3600}, // transition 161
  {1806195600U, 7200}, // transition 162
  {1824944400U, 3600}, // transition 163
  {1837645200U, 7200}, // transition 164
  {1856394000U, 3600}, // transition 165
  {1869094800U, 7200}, // transition 166
  {1887843600U, 3600}, // transition 167
  {1901149200U, 7200}, // transition 168
  {1919293200U, 3600}, // transition 169
  {1932598800U, 7200}, // transition 170
  {1950742800U, 3600}, // transition 171
  {1964048400U, 7200}, // transition 172
  {1982797200U, 3600}, // transition 173
  {1995498000U, 7200}, // transition 174
  {2014246800U, 3600}, // transition 175
  {2026947600U, 7200}, // transition 176
  {2045696400U, 3600}, // transition 177
  {2058397200U, 7200}, // transition 178
  {2077146000U, 3600}, // transition 179
  {2090451600U, 7200}, // transition 180
  {2108595600U, 3600}, // transition 181
  {2121901200U, 7200}, // transition 182
  {2140045200U, 3600}, // transition 183
  {2153350800U, 7200}, // transition 184
  {2172099600U, 3600}, // transition 185
  {2184800400U, 7200}, // transition 186
  {2203549200U, 3600}, // transition 187
  {2216250000U, 7200}, // transition 188
  {2234998800U, 3600}, // transition 189
  {2248304400U, 7200}, // transition 190
  {2266448400U, 3600}, // transition 191
  {2279754000U, 7200}, // transition 192
  {2297898000U, 3600}, // transition 193
  {2311203600U, 7200}, // transition 194
  {2329347600U, 3600}, // transition 195
  {2342653200U, 7200}, // transition 196
  {2361402000U, 3600}, // transition 197
  {2374102800U, 7200}, // transition 198
  {2392851600U, 3600}, // transition 199
  {2405552400U, 7200}, // transition 200
  {2424301200U, 3600}, // transition 201
  {2437606800U, 7200}, // transition 202
  {2455750800U, 3600}, // transition 203
  {2469056400U, 7200}, // transition 204
  {2487200400U, 3600}, // transition 205
  {2500506000U, 7200}, // transition 206
  {2519254800U, 3600}, // transition 207
  {2531955600U, 7200}, // transition 208
  {2550704400U, 3600}, // transition 209
  {2563405200U, 7200}, // transition 210
  {2582154000U, 3600}, // transition 211
  {2595459600U, 7200}, // transition 212
  {2613603600U, 3600}, // transition 213
  {2626909200U, 7200}, // transition 214
  {2645053200U, 3600}, // transition 215
  {2658358800U, 7200}, // transition 216
  {2676502800U, 3600}, // transition 217
  {2689808400U, 7200}, // transition 218
  {2708557200U, 3600}, // transition 219
  {2721258000U, 7200}, // transition 220
  {2740006800U, 3600}, // transition 221
  {2752707600U, 7200}, // transition 222
  {2771456400U, 3600}, // transition 223
  {2784762000U, 7200}, // transition 224
  {2802906000U, 3600}, // transition 225
  {2816211600U, 7200}, // transition 226
  {2834355600U, 3600}, // transition 227
  {2847661200U, 7200}, // transition 228
  {2866410000U, 3600}, // transition 229
  {2879110800U, 7200}, // transition 230
  {2897859600U, 3600}, // transition 231
  {2910560400U, 7200}, // transition 232
  {2929309200U, 3600}, // transition 233
  {2942010000U, 7200}, // transition 234
  {2960758800U, 3600}, // transition 235
  {2974064400U, 7200}, // transition 236
  {2992208400U, 3600}, // transition 237
  {3005514000U, 7200}, // transition 238
  {3023658000U, 3600}, // transition 239
  {3036963600U, 7200}, // transition 240
  {3055712400U, 3600}, // transition 241
  {3068413200U, 7200}, // transition 242
  {3087162000U, 3600}, // transition 243
  {3099862800U, 7200}, // transition 244
  {3118611600U, 3600}, // transition 245
  {3131917200U, 7200}, // transition 246
  {3150061200U, 3600}, // transition 247
  {3163366800U, 7200}, // transition 248
  {3181510800U, 3600}, // transition 249
  {3194816400U, 7200}, // transition 250
  {3212960400U, 3600}, // transition 251
  {3226266000U, 7200}, // transition 252
  {3245014800U, 3600}, // transition 253
  {3257715600U, 7200}, // transition 254
  {3276464400U, 3600}, // transition 255
  {3289165200U, 7200}, // transition 256
  {3307914000U, 3600}, // transition 257
  {3321219600U, 7200}, // transition 258
  {3339363600U, 3600}, // transition 259
  {3352669200U, 7200}, // transition 260
  {3370813200U, 3600}, // transition 261
  {3384118800U, 7200}, // transition 262
  {3402867600U, 3600}, // transition 263
  {3415568400U, 7200}, // transition 264
  {3434317200U, 3600}, // transition 265
  {3447018000U, 7200}, // transition 266
  {3465766800U, 3600}, // transition 267
  {3479072400U, 7200}, // transition 268
  {3497216400U, 3600}, // transition 269
  {3510522000U, 7200}, // transition 270
  {3528666000U, 3600}, // transition 271
  {3541971600U, 7200}, // transition 272
  {3560115600U, 3600}, // transition 273
  {3573421200U, 7200}, // transition 274
  {3592170000U, 3600}, // transition 275
  {3604870800U, 7200}, // transition 276
  {3623619600U, 3600}, // transition 277
  {3636320400U, 7200}, // transition 278
  {3655069200U, 3600}, // transition 279
  {3668374800U, 7200}, // transition 280
  {3686518800U, 3600}, // transition 281
  {3699824400U, 7200}, // transition 282
  {3717968400U, 3600}, // transition 283
  {3731274000U, 7200}, // transition 284
  {3750022800U, 3600}, // transition 285
  {3762723600U, 7200}, // transition 286
  {3781472400U, 3600}, // transition 287
  {3794173200U, 7200}, // transition 288
  {3812922000U, 3600}, // transition 289
  {3825622800U, 7200}, // transition 290
  {3844371600U, 3600}, // transition 291
  {3857677200U, 7200}, // transition 292
  {3875821200U, 3600}, // transition 293
  {3889126800U, 7200}, // transition 294
  {3907270800U, 3600}, // transition 295
  {3920576400U, 7200}, // transition 296
  {3939325200U, 3600}, // transition 297
  {3952026000U, 7200}, // transition 298
  {3970774800U, 3600}, // transition 299
  {3983475600U, 7200}, // transition 300
  {4002224400U, 3600}, // transition 301
  {4015530000U, 7200}, // transition 302
  {4033674000U, 3600}, // transition 303
  {4046979600U, 7200}, // transition 304
  {4065123600U, 3600}, // transition 305
  {4078429200U, 7200}, // transition 306
  {4096573200U, 3600}, // transition 307
  {4109878800U, 7200}, // transition 308
  {4128627600U, 3600}, // transition 309
  {1767225600U, -36000}, // transition 310
  {1772971200U, -32400}, // transition 311
  {1793530800U, -36000}, // transition 312
  {1805025600U, -32400}, // transition 313
  {1825585200U, -36000}, // transition 314
  {1836475200U, -32400}, // transition 315
  {1857034800U, -36000}, // transition 316
  {1867924800U, -32400}, // transition 317
  {1888484400U, -36000}, // transition 318
  {1899374400U, -32400}, // transition 319
  {1919934000U, -36000}, // transition 320
  {1930824000U, -32400}, // transition 321
  {1951383600U, -36000}, // transition 322
  {1962878400U, -32400}, // transition 323
  {1983438000U, -36000}, // transition 324
  {1994328000U, -32400}, // transition 325
  {2014887600U, -36000}, // transition 326
  {2025777600U, -32400}, // transition 327
  {2046337200U, -36000}, // transition 328
  {2057227200U, -32400}, // transition 329
  {2077786800U, -36000}, // transition 330
  {2088676800U, -32400}, // transition 331
  {2109236400U, -36000}, // transition 332
  {2120126400U, -32400}, // transition 333
  {2140686000U, -36000}, // transition 334
  {2152180800U, -32400}, // transition 335
  {2172740400U, -36000}, // transition 336
  {2183630400U, -32400}, // transition 337
  {2204190000U, -36000}, // transition 338
  {2215080000U, -32400}, // transition 339
  {2235639600U, -36000}, // transition 340
  {2246529600U, -32400}, // transition 341
  {2267089200U, -36000}, // transition 342
  {2277979200U, -32400}, // transition 343
  {2298538800U, -36000}, // transition 344
  {2309428800U, -32400}, // transition 345
  {2329988400U, -36000}, // transition 346
  {2341483200U, -32400}, // transition 347
  {2362042800U, -36000}, // transition 348
  {2372932800U, -32400}, // transition 349
  {2393492400U, -36000}, // transition 350
  {2404382400U, -32400}, // transition 351
  {2424942000U, -36000}, // transition 352
  {2435832000U, -32400}, // transition 353
  {2456391600U, -36000}, // transition 354
  {2467281600U, -32400}, // transition 355
  {2487841200U, -36000}, // transition 356
  {2499336000U, -32400}, // transition 357
  {2519895600U, -36000}, // transition 358
  {2530785600U, -32400}, // transition 359
  {2551345200U, -36000}, // transition 360
  {2562235200U, -32400}, // transition 361
  {2582794800U, -36000}, // transition 362
  {2593684800U, -32400}, // transition 363
  {2614244400U, -36000}, // transition 364
  {2625134400U, -32400}, // transition 365
  {2645694000U, -36000}, // transition 366
  {2656584000U, -32400}, // transition 367
  {2677143600U, -36000}, // transition 368
  {2688638400U, -32400}, // transition 369
  {2709198000U, -36000}, // transition 370
  {2720088000U, -32400}, // transition 371
  {2740647600U, -36000}, // transition 372
  {2751537600U, -32400}, // transition 373
  {2772097200U, -36000}, // transition 374
  {2782987200U, -32400}, // transition 375
  {2803546800U, -36000}, // transition 376
  {2814436800U, -32400}, // transition 377
  {2834996400U, -36000}, // transition 378
  {2846491200U, -32400}, // transition 379
  {2867050800U, -36000}, // transition 380
  {2877940800U, -32400}, // transition 381
  {2898500400U, -36000}, // transition 382
  {2909390400U, -32400}, // transition 383
  {2929950000U, -36000}, // transition 384
  {2940840000U, -32400}, // transition 385
  {2961399600U, -36000}, // transition 386
  {2972289600U, -32400}, // transition 387
  {2992849200U, -36000}, // transition 388
  {3003739200U, -32400}, // transition 389
  {3024298800U, -36000}, // transition 390
  {3035793600U, -32400}, // transition 391
  {3056353200U, -36000}, // transition 392
  {3067243200U, -32400}, // transition 393
  {3087802800U, -36000}, // transition 394
  {3098692800U, -32400}, // transition 395
  {3119252400U, -36000}, // transition 396
  {3130142400U, -32400}, // transition 397
  {3150702000U, -36000}, // transition 398
  {3161592000U, -32400}, // transition 399
  {3182151600U, -36000}, // transition 400
  {3193041600U, -32400}, // transition 401
  {3213601200U, -36000}, // transition 402
  {3225096000U, -32400}, // transition 403
  {3245655600U, -36000}, // transition 404
  {3256545600U, -32400}, // transition 405
  {3277105200U, -36000}, // transition 406
  {3287995200U, -32400}, // transition 407
  {3308554800U, -36000}, // transition 408
  {3319444800U, -32400}, // transition 409
  {3340004400U, -36000}, // transition 410
  {3350894400U, -32400}, // transition 411
  {3371454000U, -36000}, // transition 412
  {3382948800U, -32400}, // transition 413
  {3403508400U, -36000}, // transition 414
  {3414398400U, -32400}, // transition 415
  {3434958000U, -36000}, // transition 416
  {3445848000U, -32400}, // transition 417
  {3466407600U, -36000}, // transition 418
  {3477297600U, -32400}, // transition 419
  {3497857200U, -36000}, // transition 420
  {3508747200U, -32400}, // transition 421
  {3529306800U, -36000}, // transition 422
  {3540196800U, -32400}, // transition 423
  {3560756400U, -36000}, // transition 424
  {3572251200U, -32400}, // transition 425
  {3592810800U, -36000}, // transition 426
  {3603700800U, -32400}, // transition 427
  {3624260400U, -36000}, // transition 428
  {3635150400U, -32400}, // transition 429
  {3655710000U, -36000}, // transition 430
  {3666600000U, -32400}, // transition 431
  {3687159600U, -36000}, // transition 432
  {3698049600U, -32400}, // transition 433
  {3718609200U, -36000}, // transition 434
  {3730104000U, -32400}, // transition 435
  {3750663600U, -36000}, // transition 436
  {3761553600U, -32400}, // transition 437
  {3782113200U, -36000}, // transition 438
  {3793003200U, -32400}, // transition 439
  {3813562800U, -36000}, // transition 440
  {3824452800U, -32400}, // transition 441
  {3845012400U, -36000}, // transition 442
  {3855902400U, -32400}, // transition 443
  {3876462000U, -36000}, // transition 444
  {3887352000U, -32400}, // transition 445
  {3907911600U, -36000}, // transition 446
  {3919406400U, -32400}, // transition 447
  {3939966000U, -36000}, // transition 448
  {3950856000U, -32400}, // transition 449
  {3971415600U, -36000}, // transition 450
  {3982305600U, -32400}, // transition 451
  {4002865200U, -36000}, // transition 452
  {4013755200U, -32400}, // transition 453
  {4034314800U, -36000}, // transition 454
  {4045204800U, -32400}, // transition 455
  {4065764400U, -36000}, // transition 456
  {4076654400U, -32400}, // transition 457
  {4097214000U, -36000}, // transition 458
  {4108708800U, -32400}, // transition 459
  {4129268400U, -36000}, // transition 460
  {1767225600U, -32400}, // transition 461
  {1772967600U, -28800}, // transition 462
  {1793527200U, -32400}, // transition 463
  {1805022000U, -28800}, // transition 464
  {1825581600U, -32400}, // transition 465
  {1836471600U, -28800}, // transition 466
  {1857031200U, -32400}, // transition 467
  {1867921200U, -28800}, // transition 468
  {1888480800U, -32400}, // transition 469
  {1899370800U, -28800}, // transition 470
  {1919930400U, -32400}, // transition 471
  {1930820400U, -28800}, // transition 472
  {1951380000U, -32400}, // transition 473
  {1962874800U, -28800}, // transition 474
  {1983434400U, -32400}, // transition 475
  {1994324400U, -28800}, // transition 476
  {2014884000U, -32400}, // transition 477
  {2025774000U, -28800}, // transition 478
  {2046333600U, -32400}, // transition 479
  {2057223600U, -28800}, // transition 480
  {2077783200U, -32400}, // transition 481
  {2088673200U, -28800}, // transition 482
  {2109232800U, -32400}, // transition 483
  {2120122800U, -28800}, // transition 484
  {2140682400U, -32400}, // transition 485
  {2152177200U, -28800}, // transition 486
  {2172736800U, -32400}, // transition 487
  {2183626800U, -28800}, // transition 488
  {2204186400U, -32400}, // transition 489
  {2215076400U, -28800}, // transition 490
  {2235636000U, -32400}, // transition 491
  {2246526000U, -28800}, // transition 492
  {2267085600U, -32400}, // transition 493
  {2277975600U, -28800}, // transition 494
  {2298535200U, -32400}, // transition 495
  {2309425200U, -28800}, // transition 496
  {2329984800U, -32400}, // transition 497
  {2341479600U, -28800}, // transition 498
  {2362039200U, -32400}, // transition 499
  {2372929200U, -28800}, // transition 500
  {2393488800U, -32400}, // transition 501
  {2404378800U, -28800}, // transition 502
  {2424938400U, -32400}, // transition 503
  {2435828400U, -28800}, // transition 504
  {2456388000U, -32400}, // transition 505
  {2467278000U, -28800}, // transition 506
  {2487837600U, -32400}, // transition 507
  {2499332400U, -28800}, // transition 508
  {2519892000U, -32400}, // transition 509
  {2530782000U, -28800}, // transition 510
  {2551341600U, -32400}, // transition 511
  {2562231600U, -28800}, // transition 512
  {2582791200U, -32400}, // transition 513
  {2593681200U, -28800}, // transition 514
  {2614240800U, -32400}, // transition 515
  {2625130800U, -28800}, // transition 516
  {2645690400U, -32400}, // transition 517
  {2656580400U, -28800}, // transition 518
  {2677140000U, -32400}, // transition 519
  {2688634800U, -28800}, // transition 520
  {2709194400U, -32400}, // transition 521
  {2720084400U, -28800}, // transition 522
  {2740644000U, -32400}, // transition 523
  {2751534000U, -28800}, // transition 524
  {2772093600U, -32400}, // transition 525
  {2782983600U, -28800}, // transition 526
  {2803543200U, -32400}, // transition 527
  {2814433200U, -28800}, // transition 528
  {2834992800U, -32400}, // transition 529
  {2846487600U, -28800}, // transition 530
  {2867047200U, -32400}, // transition 531
  {2877937200U, -28800}, // transition 532
  {2898496800U, -32400}, // transition 533
  {2909386800U, -28800}, // transition 534
  {2929946400U, -32400}, // transition 535
  {2940836400U, -28800}, // transition 536
  {2961396000U, -32400}, // transition 537
  {2972286000U, -28800}, // transition 538
  {2992845600U, -32400}, // transition 539
  {3003735600U, -28800}, // transition 540
  {3024295200U, -32400}, // transition 541
  {3035790000U, -28800}, // transition 542
  {3056349600U, -32400}, // transition 543
  {3067239600U, -28800}, // transition 544
  {3087799200U, -32400}, // transition 545
  {3098689200U, -28800}, // transition 546
  {3119248800U, -32400}, // transition 547
  {3130138800U, -28800}, // transition 548
  {3150698400U, -32400}, // transition 549
  {3161588400U, -28800}, // transition 550
  {3182148000U, -32400}, // transition 551
  {3193038000U, -28800}, // transition 552
  {3213597600U, -32400}, // transition 553
  {3225092400U, -28800}, // transition 554
  {3245652000U, -32400}, // transition 555
  {3256542000U, -28800}, // transition 556
  {3277101600U, -32400}, // transition 557
  {3287991600U, -28800}, // transition 558
  {3308551200U, -32400}, // transition 559
  {3319441200U, -28800}, // transition 560
  {3340000800U, -32400}, // transition 561
  {3350890800U, -28800}, // transition 562
  {3371450400U, -32400}, // transition 563
  {3382945200U, -28800}, // transition 564
  {3403504800U, -32400}, // transition 565
  {3414394800U, -28800}, // transition 566
  {3434954400U, -32400}, // transition 567
  {3445844400U, -28800}, // transition 568
  {3466404000U, -32400}, // transition 569
  {3477294000U, -28800}, // transition 570
  {3497853600U, -32400}, // transition 571
  {3508743600U, -28800}, // transition 572
  {3529303200U, -32400}, // transition 573
  {3540193200U, -28800}, // transition 574
  {3560752800U, -32400}, // transition 575
  {3572247600U, -28800}, // transition 576
  {3592807200U, -32400}, // transition 577
  {3603697200U, -28800}, // transition 578
  {3624256800U, -32400}, // transition 579
  {3635146800U, -28800}, // transition 580
  {3655706400U, -32400}, // transition 581
  {3666596400U, -28800}, // transition 582
  {3687156000U, -32400}, // transition 583
  {3698046000U, -28800}, // transition 584
  {3718605600U, -32400}, // transition 585
  {3730100400U, -28800}, // transition 586
  {3750660000U, -32400}, // transition 587
  {3761550000U, -28800}, // transition 588
  {3782109600U, -32400}, // transition 589
  {3792999600U, -28800}, // transition 590
  {3813559200U, -32400}, // transition 591
  {3824449200U, -28800}, // transition 592
  {3845008800U, -32400}, // transition 593
  {3855898800U, -28800}, // transition 594
  {3876458400U, -32400}, // transition 595
  {3887348400U, -28800}, // transition 596
  {3907908000U, -32400}, // transition 597
  {3919402800U, -28800}, // transition 598
  {3939962400U, -32400}, // transition 599
  {3950852400U, -28800}, // transition 600
  {3971412000U, -32400}, // transition 601
  {3982302000U, -28800}, // transition 602
  {4002861600U, -32400}, // transition 603
  {4013751600U, -28800}, // transition 604
  {4034311200U, -32400}, // transition 605
  {4045201200U, -28800}, // transition 606
  {4065760800U, -32400}, // transition 607
  {4076650800U, -28800}, // transition 608
  {4097210400U, -32400}, // transition 609
  {4108705200U, -28800}, // transition 610
  {4129264800U, -32400}, // transition 611
  {1767225600U, -14400}, // transition 612
  {1767225600U, -10800}, // transition 613
  {1767225600U, -18000}, // transition 614
  {1767225600U, -21600}, // transition 615
  {1767225600U, -25200}, // transition 616
  {1772960400U, -21600}, // transition 617
  {1793520000U, -25200}, // transition 618
  {1805014800U, -21600}, // transition 619
  {1825574400U, -25200}, // transition 620
  {1836464400U, -21600}, // transition 621
  {1857024000U, -25200}, // transition 622
  {1867914000U, -21600}, // transition 623
  {1888473600U, -25200}, // transition 624
  {1899363600U, -21600}, // transition 625
  {1919923200U, -25200}, // transition 626
  {1930813200U, -21600}, // transition 627
  {1951372800U, -25200}, // transition 628
  {1962867600U, -21600}, // transition 629
  {1983427200U, -25200}, // transition 630
  {1994317200U, -21600}, // transition 631
  {2014876800U, -25200}, // transition 632
  {2025766800U, -21600}, // transition 633
  {2046326400U, -25200}, // transition 634
  {2057216400U, -21600}, // transition 635
  {2077776000U, -25200}, // transition 636
  {2088666000U, -21600}, // transition 637
  {2109225600U, -25200}, // transition 638
  {2120115600U, -21600}, // transition 639
  {2140675200U, -25200}, // transition 640
  {2152170000U, -21600}, // transition 641
  {2172729600U, -25200}, // transition 642
  {2183619600U, -21600}, // transition 643
  {2204179200U, -25200}, // transition 644
  {2215069200U, -21600}, // transition 645
  {2235628800U, -25200}, // transition 646
  {2246518800U, -21600}, // transition 647
  {2267078400U, -25200}, // transition 648
  {2277968400U, -21600}, // transition 649
  {2298528000U, -25200}, // transition 650
  {2309418000U, -21600}, // transition 651
  {2329977600U, -25200}, // transition 652
  {2341472400U, -21600}, // transition 653
  {2362032000U, -25200}, // transition 654
  {2372922000U, -21600}, // transition 655
  {2393481600U, -25200}, // transition 656
  {2404371600U, -21600}, // transition 657
  {2424931200U, -25200}, // transition 658
  {2435821200U, -21600}, // transition 659
  {2456380800U, -25200}, // transition 660
  {2467270800U, -21600}, // transition 661
  {2487830400U, -25200}, // transition 662
  {2499325200U, -21600}, // transition 663
  {2519884800U, -25200}, // transition 664
  {2530774800U, -21600}, // transition 665
  {2551334400U, -25200}, // transition 666
  {2562224400U, -21600}, // transition 667
  {2582784000U, -25200}, // transition 668
  {2593674000U, -21600}, // transition 669
  {2614233600U, -25200}, // transition 670
  {2625123600U, -21600}, // transition 671
  {2645683200U, -25200}, // transition 672
  {2656573200U, -21600}, // transition 673
  {2677132800U, -25200}, // transition 674
  {2688627600U, -21600}, // transition 675
  {2709187200U, -25200}, // transition 676
  {2720077200U, -21600}, // transition 677
  {2740636800U, -25200}, // transition 678
  {2751526800U, -21600}, // transition 679
  {2772086400U, -25200}, // transition 680
  {2782976400U, -21600}, // transition 681
  {2803536000U, -25200}, // transition 682
  {2814426000U, -21600}, // transition 683
  {2834985600U, -25200}, // transition 684
  {2846480400U, -21600}, // transition 685
  {2867040000U, -25200}, // transition 686
  {2877930000U, -21600}, // transition 687
  {2898489600U, -25200}, // transition 688
  {2909379600U, -21600}, // transition 689
  {2929939200U, -25200}, // transition 690
  {2940829200U, -21600}, // transition 691
  {2961388800U, -25200}, // transition 692
  {2972278800U, -21600}, // transition 693
  {2992838400U, -25200}, // transition 694
  {3003728400U, -21600}, // transition 695
  {3024288000U, -25200}, // transition 696
  {3035782800U, -21600}, // transition 697
  {3056342400U, -25200}, // transition 698
  {3067232400U, -21600}, // transition 699
  {3087792000U, -25200}, // transition 700
  {3098682000U, -21600}, // transition 701
  {3119241600U, -25200}, // transition 702
  {3130131600U, -21600}, // transition 703
  {3150691200U, -25200}, // transition 704
  {3161581200U, -21600}, // transition 705
  {3182140800U, -25200}, // transition 706
  {3193030800U, -21600}, // transition 707
  {3213590400U, -25200}, // transition 708
  {3225085200U, -21600}, // transition 709
  {3245644800U, -25200}, // transition 710
  {3256534800U, -21600}, // transition 711
  {3277094400U, -25200}, // transition 712
  {3287984400U, -21600}, // transition 713
  {3308544000U, -25200}, // transition 714
  {3319434000U, -21600}, // transition 715
  {3339993600U, -25200}, // transition 716
  {3350883600U, -21600}, // transition 717
  {3371443200U, -25200}, // transition 718
  {3382938000U, -21600}, // transition 719
  {3403497600U, -25200}, // transition 720
  {3414387600U, -21600}, // transition 721
  {3434947200U, -25200}, // transition 722
  {3445837200U, -21600}, // transition 723
  {3466396800U, -25200}, // transition 724
  {3477286800U, -21600}, // transition 725
  {3497846400U, -25200}, // transition 726
  {3508736400U, -21600}, // transition 727
  {3529296000U, -25200}, // transition 728
  {3540186000U, -21600}, // transition 729
  {3560745600U, -25200}, // transition 730
  {3572240400U, -21600}, // transition 731
  {3592800000U, -25200}, // transition 732
  {3603690000U, -21600}, // transition 733
  {3624249600U, -25200}, // transition 734
  {3635139600U, -21600}, // transition 735
  {3655699200U, -25200}, // transition 736
  {3666589200U, -21600}, // transition 737
  {3687148800U, -25200}, // transition 738
  {3698038800U, -21600}, // transition 739
  {3718598400U, -25200}, // transition 740
  {3730093200U, -21600}, // transition 741
  {3750652800U, -25200}, // transition 742
  {3761542800U, -21600}, // transition 743
  {3782102400U, -25200}, // transition 744
  {3792992400U, -21600}, // transition 745
  {3813552000U, -25200}, // transition 746
  {3824442000U, -21600}, // transition 747
  {3845001600U, -25200}, // transition 748
  {3855891600U, -21600}, // transition 749
  {3876451200U, -25200}, // transition 750
  {3887341200U, -21600}, // transition 751
  {3907900800U, -25200}, // transition 752
  {3919395600U, -21600}, // transition 753
  {3939955200U, -25200}, // transition 754
  {3950845200U, -21600}, // transition 755
  {3971404800U, -25200}, // transition 756
  {3982294800U, -21600}, // transition 757
  {4002854400U, -25200}, // transition 758
  {4013744400U, -21600}, // transition 759
  {4034304000U, -25200}, // transition 760
  {4045194000U, -21600}, // transition 761
  {4065753600U, -25200}, // transition 762
  {4076643600U, -21600}, // transition 763
  {4097203200U, -25200}, // transition 764
  {4108698000U, -21600}, // transition 765
  {4129257600U, -25200}, // transition 766
  {1767225600U, -21600}, // transition 767
  {1772956800U, -18000}, // transition 768
  {1793516400U, -21600}, // transition 769
  {1805011200U, -18000}, // transition 770
  {1825570800U, -21600}, // transition 771
  {1836460800U, -18000}, // transition 772
  {1857020400U, -21600}, // transition 773
  {1867910400U, -18000}, // transition 774
  {1888470000U, -21600}, // transition 775
  {1899360000U, -18000}, // transition 776
  {1919919600U, -21600}, // transition 777
  {1930809600U, -18000}, // transition 778
  {1951369200U, -21600}, // transition 779
  {1962864000U, -18000}, // transition 780
  {1983423600U, -21600}, // transition 781
  {1994313600U, -18000}, // transition 782
  {2014873200U, -21600}, // transition 783
  {2025763200U, -18000}, // transition 784
  {2046322800U, -21600}, // transition 785
  {2057212800U, -18000}, // transition 786
  {2077772400U, -21600}, // transition 787
  {2088662400U, -18000}, // transition 788
  {2109222000U, -21600}, // transition 789
  {2120112000U, -18000}, // transition 790
  {2140671600U, -21600}, // transition 791
  {2152166400U, -18000}, // transition 792
  {2172726000U, -21600}, // transition 793
  {2183616000U, -18000}, // transition 794
  {2204175600U, -21600}, // transition 795
  {2215065600U, -18000}, // transition 796
  {2235625200U, -21600}, // transition 797
  {2246515200U, -18000}, // transition 798
  {2267074800U, -21600}, // transition 799
  {2277964800U, -18000}, // transition 800
  {2298524400U, -21600}, // transition 801
  {2309414400U, -18000}, // transition 802
  {2329974000U, -21600}, // transition 803
  {2341468800U, -18000}, // transition 804
  {2362028400U, -21600}, // transition 805
  {2372918400U, -18000}, // transition 806
  {2393478000U, -21600}, // transition 807
  {2404368000U, -18000}, // transition 808
  {2424927600U, -21600}, // transition 809
  {2435817600U, -18000}, // transition 810
  {2456377200U, -21600}, // transition 811
  {2467267200U, -18000}, // transition 812
  {2487826800U, -21600}, // transition 813
  {2499321600U, -18000}, // transition 814
  {2519881200U, -21600}, // transition 815
  {2530771200U, -18000}, // transition 816
  {2551330800U, -21600}, // transition 817
  {2562220800U, -18000}, // transition 818
  {2582780400U, -21600}, // transition 819
  {2593670400U, -18000}, // transition 820
  {2614230000U, -21600}, // transition 821
  {2625120000U, -18000}, // transition 822
  {2645679600U, -21600}, // transition 823
  {2656569600U, -18000}, // transition 824
  {2677129200U, -21600}, // transition 825
  {2688624000U, -18000}, // transition 826
  {2709183600U, -21600}, // transition 827
  {2720073600U, -18000}, // transition 828
  {2740633200U, -21600}, // transition 829
  {2751523200U, -18000}, // transition 830
  {2772082800U, -21600}, // transition 831
  {2782972800U, -18000}, // transition 832
  {2803532400U, -21600}, // transition 833
  {2814422400U, -18000}, // transition 834
  {2834982000U, -21600}, // transition 835
  {2846476800U, -18000}, // transition 836
  {2867036400U, -21600}, // transition 837
  {2877926400U, -18000}, // transition 838
  {2898486000U, -21600}, // transition 839
  {2909376000U, -18000}, // transition 840
  {2929935600U, -21600}, // transition 841
  {2940825600U, -18000}, // transition 842
  {2961385200U, -21600}, // transition 843
  {2972275200U, -18000}, // transition 844
  {2992834800U, -21600}, // transition 845
  {3003724800U, -18000}, // transition 846
  {3024284400U, -21600}, // transition 847
  {3035779200U, -18000}, // transition 848
  {3056338800U, -21600}, // transition 849
  {3067228800U, -18000}, // transition 850
  {3087788400U, -21600}, // transition 851
  {3098678400U, -18000}, // transition 852
  {3119238000U, -21600}, // transition 853
  {3130128000U, -18000}, // transition 854
  {3150687600U, -21600}, // transition 855
  {3161577600U, -18000}, // transition 856
  {3182137200U, -21600}, // transition 857
  {3193027200U, -18000}, // transition 858
  {3213586800U, -21600}, // transition 859
  {3225081600U, -18000}, // transition 860
  {3245641200U, -21600}, // transition 861
  {3256531200U, -18000}, // transition 862
  {3277090800U, -21600}, // transition 863
  {3287980800U, -18000}, // transition 864
  {3308540400U, -21600}, // transition 865
  {3319430400U, -18000}, // transition 866
  {3339990000U, -21600}, // transition 867
  {3350880000U, -18000}, // transition 868
  {3371439600U, -21600}, // transition 869
  {3382934400U, -18000}, // transition 870
  {3403494000U, -21600}, // transition 871
  {3414384000U, -18000}, // transition 872
  {3434943600U, -21600}, // transition 873
  {3445833600U, -18000}, // transition 874
  {3466393200U, -21600}, // transition 875
  {3477283200U, -18000}, // transition 876
  {3497842800U, -21600}, // transition 877
  {3508732800U, -18000}, // transition 878
  {3529292400U, -21600}, // transition 879
  {3540182400U, -18000}, // transition 880
  {3560742000U, -21600}, // transition 881
  {3572236800U, -18000}, // transition 882
  {3592796400U, -21600}, // transition 883
  {3603686400U, -18000}, // transition 884
  {3624246000U, -21600}, // transition 885
  {3635136000U, -18000}, // transition 886
  {3655695600U, -21600}, // transition 887
  {3666585600U, -18000}, // transition 888
  {3687145200U, -21600}, // transition 889
  {3698035200U, -18000}, // transition 890
  {3718594800U, -21600}, // transition 891
  {3730089600U, -18000}, // transition 892
  {3750649200U, -21600}, // transition 893
  {3761539200U, -18000}, // transition 894
  {3782098800U, -21600}, // transition 895
  {3792988800U, -18000}, // transition 896
  {3813548400U, -21600}, // transition 897
  {3824438400U, -18000}, // transition 898
  {3844998000U, -21600}, // transition 899
  {3855888000U, -18000}, // transition 900
  {3876447600U, -21600}, // transition 901
  {3887337600U, -18000}, // transition 902
  {3907897200U, -21600}, // transition 903
  {3919392000U, -18000}, // transition 904
  {3939951600U, -21600}, // transition 905
  {3950841600U, -18000}, // transition 906
  {3971401200U, -21600}, // transition 907
  {3982291200U, -18000}, // transition 908
  {4002850800U, -21600}, // transition 909
  {4013740800U, -18000}, // transition 910
  {4034300400U, -21600}, // transition 911
  {4045190400U, -18000}, // transition 912
  {4065750000U, -21600}, // transition 913
  {4076640000U, -18000}, // transition 914
  {4097199600U, -21600}, // transition 915
  {4108694400U, -18000}, // transition 916
  {4129254000U, -21600}, // transition 917
  {1767225600U, -25200}, // transition 918
  {1767225600U, -18000}, // transition 919
  {1772953200U, -14400}, // transition 920
  {1793512800U, -18000}, // transition 921
  {1805007600U, -14400}, // transition 922
  {1825567200U, -18000}, // transition 923
  {1836457200U, -14400}, // transition 924
  {1857016800U, -18000}, // transition 925
  {1867906800U, -14400}, // transition 926
  {1888466400U, -18000}, // transition 927
  {1899356400U, -14400}, // transition 928
  {1919916000U, -18000}, // transition 929
  {1930806000U, -14400}, // transition 930
  {1951365600U, -18000}, // transition 931
  {1962860400U, -14400}, // transition 932
  {1983420000U, -18000}, // transition 933
  {1994310000U, -14400}, // transition 934
  {2014869600U, -18000}, // transition 935
  {2025759600U, -14400}, // transition 936
  {2046319200U, -18000}, // transition 937
  {2057209200U, -14400}, // transition 938
  {2077768800U, -18000}, // transition 939
  {2088658800U, -14400}, // transition 940
  {2109218400U, -18000}, // transition 941
  {2120108400U, -14400}, // transition 942
  {2140668000U, -18000}, // transition 943
  {2152162800U, -14400}, // transition 944
  {2172722400U, -18000}, // transition 945
  {2183612400U, -14400}, // transition 946
  {2204172000U, -18000}, // transition 947
  {2215062000U, -14400}, // transition 948
  {2235621600U, -18000}, // transition 949
  {2246511600U, -14400}, // transition 950
  {2267071200U, -18000}, // transition 951
  {2277961200U, -14400}, // transition 952
  {2298520800U, -18000}, // transition 953
  {2309410800U, -14400}, // transition 954
  {2329970400U, -18000}, // transition 955
  {2341465200U, -14400}, // transition 956
  {2362024800U, -18000}, // transition 957
  {2372914800U, -14400}, // transition 958
  {2393474400U, -18000}, // transition 959
  {2404364400U, -14400}, // transition 960
  {2424924000U, -18000}, // transition 961
  {2435814000U, -14400}, // transition 962
  {2456373600U, -18000}, // transition 963
  {2467263600U, -14400}, // transition 964
  {2487823200U, -18000}, // transition 965
  {2499318000U, -14400}, // transition 966
  {2519877600U, -18000}, // transition 967
  {2530767600U, -14400}, // transition 968
  {2551327200U, -18000}, // transition 969
  {2562217200U, -14400}, // transition 970
  {2582776800U, -18000}, // transition 971
  {2593666800U, -14400}, // transition 972
  {2614226400U, -18000}, // transition 973
  {2625116400U, -14400}, // transition 974
  {2645676000U, -18000}, // transition 975
  {2656566000U, -14400}, // transition 976
  {2677125600U, -18000}, // transition 977
  {2688620400U, -14400}, // transition 978
  {2709180000U, -18000}, // transition 979
  {2720070000U, -14400}, // transition 980
  {2740629600U, -18000}, // transition 981
  {2751519600U, -14400}, // transition 982
  {2772079200U, -18000}, // transition 983
  {2782969200U, -14400}, // transition 984
  {2803528800U, -18000}, // transition 985
  {2814418800U, -14400}, // transition 986
  {2834978400U, -18000}, // transition 987
  {2846473200U, -14400}, // transition 988
  {2867032800U, -18000}, // transition 989
  {2877922800U, -14400}, // transition 990
  {2898482400U, -18000}, // transition 991
  {2909372400U, -14400}, // transition 992
  {2929932000U, -18000}, // transition 993
  {2940822000U, -14400}, // transition 994
  {2961381600U, -18000}, // transition 995
  {2972271600U, -14400}, // transition 996
  {2992831200U, -18000}, // transition 997
  {3003721200U, -14400}, // transition 998
  {3024280800U, -18000}, // transition 999
  {3035775600U, -14400}, // transition 1000
  {3056335200U, -18000}, // transition 1001
  {3067225200U, -14400}, // transition 1002
  {3087784800U, -18000}, // transition 1003
  {3098674800U, -14400}, // transition 1004
  {3119234400U, -18000}, // transition 1005
  {3130124400U, -14400}, // transition 1006
  {3150684000U, -18000}, // transition 1007
  {3161574000U, -14400}, // transition 1008
  {3182133600U, -18000}, // transition 1009
  {3193023600U, -14400}, // transition 1010
  {3213583200U, -18000}, // transition 1011
  {3225078000U, -14400}, // transition 1012
  {3245637600U, -18000}, // transition 1013
  {3256527600U, -14400}, // transition 1014
  {3277087200U, -18000}, // transition 1015
  {3287977200U, -14400}, // transition 1016
  {3308536800U, -18000}, // transition 1017
  {3319426800U, -14400}, // transition 1018
  {3339986400U, -18000}, // transition 1019
  {3350876400U, -14400}, // transition 1020
  {3371436000U, -18000}, // transition 1021
  {3382930800U, -14400}, // transition 1022
  {3403490400U, -18000}, // transition 1023
  {3414380400U, -14400}, // transition 1024
  {3434940000U, -18000}, // transition 1025
  {3445830000U, -14400}, // transition 1026
  {3466389600U, -18000}, // transition 1027
  {3477279600U, -14400}, // transition 1028
  {3497839200U, -18000}, // transition 1029
  {3508729200U, -14400}, // transition 1030
  {3529288800U, -18000}, // transition 1031
  {3540178800U, -14400}, // transition 1032
  {3560738400U, -18000}, // transition 1033
  {3572233200U, -14400}, // transition 1034
  {3592792800U, -18000}, // transition 1035
  {3603682800U, -14400}, // transition 1036
  {3624242400U, -18000}, // transition 1037
  {3635132400U, -14400}, // transition 1038
  {3655692000U, -18000}, // transition 1039
  {3666582000U, -14400}, // transition 1040
  {3687141600U, -18000}, // transition 1041
  {3698031600U, -14400}, // transition 1042
  {3718591200U, -18000}, // transition 1043
  {3730086000U, -14400}, // transition 1044
  {3750645600U, -18000}, // transition 1045
  {3761535600U, -14400}, // transition 1046
  {3782095200U, -18000}, // transition 1047
  {3792985200U, -14400}, // transition 1048
  {3813544800U, -18000}, // transition 1049
  {3824434800U, -14400}, // transition 1050
  {3844994400U, -18000}, // transition 1051
  {3855884400U, -14400}, // transition 1052
  {3876444000U, -18000}, // transition 1053
  {3887334000U, -14400}, // transition 1054
  {3907893600U, -18000}, // transition 1055
  {3919388400U, -14400}, // transition 1056
  {3939948000U, -18000}, // transition 1057
  {3950838000U, -14400}, // transition 1058
  {3971397600U, -18000}, // transition 1059
  {3982287600U, -14400}, // transition 1060
  {4002847200U, -18000}, // transition 1061
  {4013737200U, -14400}, // transition 1062
  {4034296800U, -18000}, // transition 1063
  {4045186800U, -14400}, // transition 1064
  {4065746400U, -18000}, // transition 1065
  {4076636400U, -14400}, // transition 1066
  {4097196000U, -18000}, // transition 1067
  {4108690800U, -14400}, // transition 1068
  {4129250400U, -18000}, // transition 1069
  {1767225600U, -25200}, // transition 1070
  {1772960400U, -21600}, // transition 1071
  {1767225600U, -28800}, // transition 1072
  {1772964000U, -25200}, // transition 1073
  {1793523600U, -28800}, // transition 1074
  {1805018400U, -25200}, // transition 1075
  {1825578000U, -28800}, // transition 1076
  {1836468000U, -25200}, // transition 1077
  {1857027600U, -28800}, // transition 1078
  {1867917600U, -25200}, // transition 1079
  {1888477200U, -28800}, // transition 1080
  {1899367200U, -25200}, // transition 1081
  {1919926800U, -28800}, // transition 1082
  {1930816800U, -25200}, // transition 1083
  {1951376400U, -28800}, // transition 1084
  {1962871200U, -25200}, // transition 1085
  {1983430800U, -28800}, // transition 1086
  {1994320800U, -25200}, // transition 1087
  {2014880400U, -28800}, // transition 1088
  {2025770400U, -25200}, // transition 1089
  {2046330000U, -28800}, // transition 1090
  {2057220000U, -25200}, // transition 1091
  {2077779600U, -28800}, // transition 1092
  {2088669600U, -25200}, // transition 1093
  {2109229200U, -28800}, // transition 1094
  {2120119200U, -25200}, // transition 1095
  {2140678800U, -28800}, // transition 1096
  {2152173600U, -25200}, // transition 1097
  {2172733200U, -28800}, // transition 1098
  {2183623200U, -25200}, // transition 1099
  {2204182800U, -28800}, // transition 1100
  {2215072800U, -25200}, // transition 1101
  {2235632400U, -28800}, // transition 1102
  {2246522400U, -25200}, // transition 1103
  {2267082000U, -28800}, // transition 1104
  {2277972000U, -25200}, // transition 1105
  {2298531600U, -28800}, // transition 1106
  {2309421600U, -25200}, // transition 1107
  {2329981200U, -28800}, // transition 1108
  {2341476000U, -25200}, // transition 1109
  {2362035600U, -28800}, // transition 1110
  {2372925600U, -25200}, // transition 1111
  {2393485200U, -28800}, // transition 1112
  {2404375200U, -25200}, // transition 1113
  {2424934800U, -28800}, // transition 1114
  {2435824800U, -25200}, // transition 1115
  {2456384400U, -28800}, // transition 1116
  {2467274400U, -25200}, // transition 1117
  {2487834000U, -28800}, // transition 1118
  {2499328800U, -25200}, // transition 1119
  {2519888400U, -28800}, // transition 1120
  {2530778400U, -25200}, // transition 1121
  {2551338000U, -28800}, // transition 1122
  {2562228000U, -25200}, // transition 1123
  {2582787600U, -28800}, // transition 1124
  {2593677600U, -25200}, // transition 1125
  {2614237200U, -28800}, // transition 1126
  {2625127200U, -25200}, // transition 1127
  {2645686800U, -28800}, // transition 1128
  {2656576800U, -25200}, // transition 1129
  {2677136400U, -28800}, // transition 1130
  {2688631200U, -25200}, // transition 1131
  {2709190800U, -28800}, // transition 1132
  {2720080800U, -25200}, // transition 1133
  {2740640400U, -28800}, // transition 1134
  {2751530400U, -25200}, // transition 1135
  {2772090000U, -28800}, // transition 1136
  {2782980000U, -25200}, // transition 1137
  {2803539600U, -28800}, // transition 1138
  {2814429600U, -25200}, // transition 1139
  {2834989200U, -28800}, // transition 1140
  {2846484000U, -25200}, // transition 1141
  {2867043600U, -28800}, // transition 1142
  {2877933600U, -25200}, // transition 1143
  {2898493200U, -28800}, // transition 1144
  {2909383200U, -25200}, // transition 1145
  {2929942800U, -28800}, // transition 1146
  {2940832800U, -25200}, // transition 1147
  {2961392400U, -28800}, // transition 1148
  {2972282400U, -25200}, // transition 1149
  {2992842000U, -28800}, // transition 1150
  {3003732000U, -25200}, // transition 1151
  {3024291600U, -28800}, // transition 1152
  {3035786400U, -25200}, // transition 1153
  {3056346000U, -28800}, // transition 1154
  {3067236000U, -25200}, // transition 1155
  {3087795600U, -28800}, // transition 1156
  {3098685600U, -25200}, // transition 1157
  {3119245200U, -28800}, // transition 1158
  {3130135200U, -25200}, // transition 1159
  {3150694800U, -28800}, // transition 1160
  {3161584800U, -25200}, // transition 1161
  {3182144400U, -28800}, // transition 1162
  {3193034400U, -25200}, // transition 1163
  {3213594000U, -28800}, // transition 1164
  {3225088800U, -25200}, // transition 1165
  {3245648400U, -28800}, // transition 1166
  {3256538400U, -25200}, // transition 1167
  {3277098000U, -28800}, // transition 1168
  {3287988000U, -25200}, // transition 1169
  {3308547600U, -28800}, // transition 1170
  {3319437600U, -25200}, // transition 1171
  {3339997200U, -28800}, // transition 1172
  {3350887200U, -25200}, // transition 1173
  {3371446800U, -28800}, // transition 1174
  {3382941600U, -25200}, // transition 1175
  {3403501200U, -28800}, // transition 1176
  {3414391200U, -25200}, // transition 1177
  {3434950800U, -28800}, // transition 1178
  {3445840800U, -25200}, // transition 1179
  {3466400400U, -28800}, // transition 1180
  {3477290400U, -25200}, // transition 1181
  {3497850000U, -28800}, // transition 1182
  {3508740000U, -25200}, // transition 1183
  {3529299600U, -28800}, // transition 1184
  {3540189600U, -25200}, // transition 1185
  {3560749200U, -28800}, // transition 1186
  {3572244000U, -25200}, // transition 1187
  {3592803600U, -28800}, // transition 1188
  {3603693600U, -25200}, // transition 1189
  {3624253200U, -28800}, // transition 1190
  {3635143200U, -25200}, // transition 1191
  {3655702800U, -28800}, // transition 1192
  {3666592800U, -25200}, // transition 1193
  {3687152400U, -28800}, // transition 1194
  {3698042400U, -25200}, // transition 1195
  {3718602000U, -28800}, // transition 1196
  {3730096800U, -25200}, // transition 1197
  {3750656400U, -28800}, // transition 1198
  {3761546400U, -25200}, // transition 1199
  {3782106000U, -28800}, // transition 1200
  {3792996000U, -25200}, // transition 1201
  {3813555600U, -28800}, // transition 1202
  {3824445600U, -25200}, // transition 1203
  {3845005200U, -28800}, // transition 1204
  {3855895200U, -25200}, // transition 1205
  {3876454800U, -28800}, // transition 1206
  {3887344800U, -25200}, // transition 1207
  {3907904400U, -28800}, // transition 1208
  {3919399200U, -25200}, // transition 1209
  {3939958800U, -28800}, // transition 1210
  {3950848800U, -25200}, // transition 1211
  {3971408400U, -28800}, // transition 1212
  {3982298400U, -25200}, // transition 1213
  {4002858000U, -28800}, // transition 1214
  {4013748000U, -25200}, // transition 1215
  {4034307600U, -28800}, // transition 1216
  {4045197600U, -25200}, // transition 1217
  {4065757200U, -28800}, // transition 1218
  {4076647200U, -25200}, // transition 1219
  {4097206800U, -28800}, // transition 1220
  {4108701600U, -25200}, // transition 1221
  {4129261200U, -28800}, // transition 1222
  {1767225600U, -14400}, // transition 1223
  {1772949600U, -10800}, // transition 1224
  {1793509200U, -14400}, // transition 1225
  {1805004000U, -10800}, // transition 1226
  {1825563600U, -14400}, // transition 1227
  {1836453600U, -10800}, // transition 1228
  {1857013200U, -14400}, // transition 1229
  {1867903200U, -10800}, // transition 1230
  {1888462800U, -14400}, // transition 1231
  {1899352800U, -10800}, // transition 1232
  {1919912400U, -14400}, // transition 1233
  {1930802400U, -10800}, // transition 1234
  {1951362000U, -14400}, // transition 1235
  {1962856800U, -10800}, // transition 1236
  {1983416400U, -14400}, // transition 1237
  {1994306400U, -10800}, // transition 1238
  {2014866000U, -14400}, // transition 1239
  {2025756000U, -10800}, // transition 1240
  {2046315600U, -14400}, // transition 1241
  {2057205600U, -10800}, // transition 1242
  {2077765200U, -14400}, // transition 1243
  {2088655200U, -10800}, // transition 1244
  {2109214800U, -14400}, // transition 1245
  {2120104800U, -10800}, // transition 1246
  {2140664400U, -14400}, // transition 1247
  {2152159200U, -10800}, // transition 1248
  {2172718800U, -14400}, // transition 1249
  {2183608800U, -10800}, // transition 1250
  {2204168400U, -14400}, // transition 1251
  {2215058400U, -10800}, // transition 1252
  {2235618000U, -14400}, // transition 1253
  {2246508000U, -10800}, // transition 1254
  {2267067600U, -14400}, // transition 1255
  {2277957600U, -10800}, // transition 1256
  {2298517200U, -14400}, // transition 1257
  {2309407200U, -10800}, // transition 1258
  {2329966800U, -14400}, // transition 1259
  {2341461600U, -10800}, // transition 1260
  {2362021200U, -14400}, // transition 1261
  {2372911200U, -10800}, // transition 1262
  {2393470800U, -14400}, // transition 1263
  {2404360800U, -10800}, // transition 1264
  {2424920400U, -14400}, // transition 1265
  {2435810400U, -10800}, // transition 1266
  {2456370000U, -14400}, // transition 1267
  {2467260000U, -10800}, // transition 1268
  {2487819600U, -14400}, // transition 1269
  {2499314400U, -10800}, // transition 1270
  {2519874000U, -14400}, // transition 1271
  {2530764000U, -10800}, // transition 1272
  {2551323600U, -14400}, // transition 1273
  {2562213600U, -10800}, // transition 1274
  {2582773200U, -14400}, // transition 1275
  {2593663200U, -10800}, // transition 1276
  {2614222800U, -14400}, // transition 1277
  {2625112800U, -10800}, // transition 1278
  {2645672400U, -14400}, // transition 1279
  {2656562400U, -10800}, // transition 1280
  {2677122000U, -14400}, // transition 1281
  {2688616800U, -10800}, // transition 1282
  {2709176400U, -14400}, // transition 1283
  {2720066400U, -10800}, // transition 1284
  {2740626000U, -14400}, // transition 1285
  {2751516000U, -10800}, // transition 1286
  {2772075600U, -14400}, // transition 1287
  {2782965600U, -10800}, // transition 1288
  {2803525200U, -14400}, // transition 1289
  {2814415200U, -10800}, // transition 1290
  {2834974800U, -14400}, // transition 1291
  {2846469600U, -10800}, // transition 1292
  {2867029200U, -14400}, // transition 1293
  {2877919200U, -10800}, // transition 1294
  {2898478800U, -14400}, // transition 1295
  {2909368800U, -10800}, // transition 1296
  {2929928400U, -14400}, // transition 1297
  {2940818400U, -10800}, // transition 1298
  {2961378000U, -14400}, // transition 1299
  {2972268000U, -10800}, // transition 1300
  {2992827600U, -14400}, // transition 1301
  {3003717600U, -10800}, // transition 1302
  {3024277200U, -14400}, // transition 1303
  {3035772000U, -10800}, // transition 1304
  {3056331600U, -14400}, // transition 1305
  {3067221600U, -10800}, // transition 1306
  {3087781200U, -14400}, // transition 1307
  {3098671200U, -10800}, // transition 1308
  {3119230800U, -14400}, // transition 1309
  {3130120800U, -10800}, // transition 1310
  {3150680400U, -14400}, // transition 1311
  {3161570400U, -10800}, // transition 1312
  {3182130000U, -14400}, // transition 1313
  {3193020000U, -10800}, // transition 1314
  {3213579600U, -14400}, // transition 1315
  {3225074400U, -10800}, // transition 1316
  {3245634000U, -14400}, // transition 1317
  {3256524000U, -10800}, // transition 1318
  {3277083600U, -14400}, // transition 1319
  {3287973600U, -10800}, // transition 1320
  {3308533200U, -14400}, // transition 1321
  {3319423200U, -10800}, // transition 1322
  {3339982800U, -14400}, // transition 1323
  {3350872800U, -10800}, // transition 1324
  {3371432400U, -14400}, // transition 1325
  {3382927200U, -10800}, // transition 1326
  {3403486800U, -14400}, // transition 1327
  {3414376800U, -10800}, // transition 1328
  {3434936400U, -14400}, // transition 1329
  {3445826400U, -10800}, // transition 1330
  {3466386000U, -14400}, // transition 1331
  {3477276000U, -10800}, // transition 1332
  {3497835600U, -14400}, // transition 1333
  {3508725600U, -10800}, // transition 1334
  {3529285200U, -14400}, // transition 1335
  {3540175200U, -10800}, // transition 1336
  {3560734800U, -14400}, // transition 1337
  {3572229600U, -10800}, // transition 1338
  {3592789200U, -14400}, // transition 1339
  {3603679200U, -10800}, // transition 1340
  {3624238800U, -14400}, // transition 1341
  {3635128800U, -10800}, // transition 1342
  {3655688400U, -14400}, // transition 1343
  {3666578400U, -10800}, // transition 1344
  {3687138000U, -14400}, // transition 1345
  {3698028000U, -10800}, // transition 1346
  {3718587600U, -14400}, // transition 1347
  {3730082400U, -10800}, // transition 1348
  {3750642000U, -14400}, // transition 1349
  {3761532000U, -10800}, // transition 1350
  {3782091600U, -14400}, // transition 1351
  {3792981600U, -10800}, // transition 1352
  {3813541200U, -14400}, // transition 1353
  {3824431200U, -10800}, // transition 1354
  {3844990800U, -14400}, // transition 1355
  {3855880800U, -10800}, // transition 1356
  {3876440400U, -14400}, // transition 1357
  {3887330400U, -10800}, // transition 1358
  {3907890000U, -14400}, // transition 1359
  {3919384800U, -10800}, // transition 1360
  {3939944400U, -14400}, // transition 1361
  {3950834400U, -10800}, // transition 1362
  {3971394000U, -14400}, // transition 1363
  {3982284000U, -10800}, // transition 1364
  {4002843600U, -14400}, // transition 1365
  {4013733600U, -10800}, // transition 1366
  {4034293200U, -14400}, // transition 1367
  {4045183200U, -10800}, // transition 1368
  {4065742800U, -14400}, // transition 1369
  {4076632800U, -10800}, // transition 1370
  {4097192400U, -14400}, // transition 1371
  {4108687200U, -10800}, // transition 1372
  {4129246800U, -14400}, // transition 1373
  {1767225600U, -7200}, // transition 1374
  {1774746000U, -3600}, // transition 1375
  {1792890000U, -7200}, // transition 1376
  {1806195600U, -3600}, // transition 1377
  {1824944400U, -7200}, // transition 1378
  {1837645200U, -3600}, // transition 1379
  {1856394000U, -7200}, // transition 1380
  {1869094800U, -3600}, // transition 1381
  {1887843600U, -7200}, // transition 1382
  {1901149200U, -3600}, // transition 1383
  {1919293200U, -7200}, // transition 1384
  {1932598800U, -3600}, // transition 1385
  {1950742800U, -7200}, // transition 1386
  {1964048400U, -3600}, // transition 1387
  {1982797200U, -7200}, // transition 1388
  {1995498000U, -3600}, // transition 1389
  {2014246800U, -7200}, // transition 1390
  {2026947600U, -3600}, // transition 1391
  {2045696400U, -7200}, // transition 1392
  {2058397200U, -3600}, // transition 1393
  {2077146000U, -7200}, // transition 1394
  {2090451600U, -3600}, // transition 1395
  {2108595600U, -7200}, // transition 1396
  {2121901200U, -3600}, // transition 1397
  {2140045200U, -7200}, // transition 1398
  {2153350800U, -3600}, // transition 1399
  {2172099600U, -7200}, // transition 1400
  {2184800400U, -3600}, // transition 1401
  {2203549200U, -7200}, // transition 1402
  {2216250000U, -3600}, // transition 1403
  {2234998800U, -7200}, // transition 1404
  {2248304400U, -3600}, // transition 1405
  {2266448400U, -7200}, // transition 1406
  {2279754000U, -3600}, // transition 1407
  {2297898000U, -7200}, // transition 1408
  {2311203600U, -3600}, // transition 1409
  {2329347600U, -7200}, // transition 1410
  {2342653200U, -3600}, // transition 1411
  {2361402000U, -7200}, // transition 1412
  {2374102800U, -3600}, // transition 1413
  {2392851600U, -7200}, // transition 1414
  {2405552400U, -3600}, // transition 1415
  {2424301200U, -7200}, // transition 1416
  {2437606800U, -3600}, // transition 1417
  {2455750800U, -7200}, // transition 1418
  {2469056400U, -3600}, // transition 1419
  {2487200400U, -7200}, // transition 1420
  {2500506000U, -3600}, // transition 1421
  {2519254800U, -7200}, // transition 1422
  {2531955600U, -3600}, // transition 1423
  {2550704400U, -7200}, // transition 1424
  {2563405200U, -3600}, // transition 1425
  {2582154000U, -7200}, // transition 1426
  {2595459600U, -3600}, // transition 1427
  {2613603600U, -7200}, // transition 1428
  {2626909200U, -3600}, // transition 1429
  {2645053200U, -7200}, // transition 1430
  {2658358800U, -3600}, // transition 1431
  {2676502800U, -7200}, // transition 1432
  {2689808400U, -3600}, // transition 1433
  {2708557200U, -7200}, // transition 1434
  {2721258000U, -3600}, // transition 1435
  {2740006800U, -7200}, // transition 1436
  {2752707600U, -3600}, // transition 1437
  {2771456400U, -7200}, // transition 1438
  {2784762000U, -3600}, // transition 1439
  {2802906000U, -7200}, // transition 1440
  {2816211600U, -3600}, // transition 1441
  {2834355600U, -7200}, // transition 1442
  {2847661200U, -3600}, // transition 1443
  {2866410000U, -7200}, // transition 1444
  {2879110800U, -3600}, // transition 1445
  {2897859600U, -7200}, // transition 1446
  {2910560400U, -3600}, // transition 1447
  {2929309200U, -7200}, // transition 1448
  {2942010000U, -3600}, // transition 1449
  {2960758800U, -7200}, // transition 1450
  {2974064400U, -3600}, // transition 1451
  {2992208400U, -7200}, // transition 1452
  {3005514000U, -3600}, // transition 1453
  {3023658000U, -7200}, // transition 1454
  {3036963600U, -3600}, // transition 1455
  {3055712400U, -7200}, // transition 1456
  {3068413200U, -3600}, // transition 1457
  {3087162000U, -7200}, // transition 1458
  {3099862800U, -3600}, // transition 1459
  {3118611600U, -7200}, // transition 1460
  {3131917200U, -3600}, // transition 1461
  {3150061200U, -7200}, // transition 1462
  {3163366800U, -3600}, // transition 1463
  {3181510800U, -7200}, // transition 1464
  {3194816400U, -3600}, // transition 1465
  {3212960400U, -7200}, // transition 1466
  {3226266000U, -3600}, // transition 1467
  {3245014800U, -7200}, // transition 1468
  {3257715600U, -3600}, // transition 1469
  {3276464400U, -7200}, // transition 1470
  {3289165200U, -3600}, // transition 1471
  {3307914000U, -7200}, // transition 1472
  {3321219600U, -3600}, // transition 1473
  {3339363600U, -7200}, // transition 1474
  {3352669200U, -3600}, // transition 1475
  {3370813200U, -7200}, // transition 1476
  {3384118800U, -3600}, // transition 1477
  {3402867600U, -7200}, // transition 1478
  {3415568400U, -3600}, // transition 1479
  {3434317200U, -7200}, // transition 1480
  {3447018000U, -3600}, // transition 1481
  {3465766800U, -7200}, // transition 1482
  {3479072400U, -3600}, // transition 1483
  {3497216400U, -7200}, // transition 1484
  {3510522000U, -3600}, // transition 1485
  {3528666000U, -7200}, // transition 1486
  {3541971600U, -3600}, // transition 1487
  {3560115600U, -7200}, // transition 1488
  {3573421200U, -3600}, // transition 1489
  {3592170000U, -7200}, // transition 1490
  {3604870800U, -3600}, // transition 1491
  {3623619600U, -7200}, // transition 1492
  {3636320400U, -3600}, // transition 1493
  {3655069200U, -7200}, // transition 1494
  {3668374800U, -3600}, // transition 1495
  {3686518800U, -7200}, // transition 1496
  {3699824400U, -3600}, // transition 1497
  {3717968400U, -7200}, // transition 1498
  {3731274000U, -3600}, // transition 1499
  {3750022800U, -7200}, // transition 1500
  {3762723600U, -3600}, // transition 1501
  {3781472400U, -7200}, // transition 1502
  {3794173200U, -3600}, // transition 1503
  {3812922000U, -7200}, // transition 1504
  {3825622800U, -3600}, // transition 1505
  {3844371600U, -7200}, // transition 1506
  {3857677200U, -3600}, // transition 1507
  {3875821200U, -7200}, // transition 1508
  {3889126800U, -3600}, // transition 1509
  {3907270800U, -7200}, // transition 1510
  {3920576400U, -3600}, // transition 1511
  {3939325200U, -7200}, // transition 1512
  {3952026000U, -3600}, // transition 1513
  {3970774800U, -7200}, // transition 1514
  {3983475600U, -3600}, // transition 1515
  {4002224400U, -7200}, // transition 1516
  {4015530000U, -3600}, // transition 1517
  {4033674000U, -7200}, // transition 1518
  {4046979600U, -3600}, // transition 1519
  {4065123600U, -7200}, // transition 1520
  {4078429200U, -3600}, // transition 1521
  {4096573200U, -7200}, // transition 1522
  {4109878800U, -3600}, // transition 1523
  {4128627600U, -7200}, // transition 1524
  {1767225600U, -18000}, // transition 1525
  {1772946000U, -14400}, // transition 1526
  {1825563600U, -18000}, // transition 1527
  {1836450000U, -14400}, // transition 1528
  {1857013200U, -18000}, // transition 1529
  {1867899600U, -14400}, // transition 1530
  {1888462800U, -18000}, // transition 1531
  {1899349200U, -14400}, // transition 1532
  {1919912400U, -18000}, // transition 1533
  {1930798800U, -14400}, // transition 1534
  {1951362000U, -18000}, // transition 1535
  {1962853200U, -14400}, // transition 1536
  {1983416400U, -18000}, // transition 1537
  {1994302800U, -14400}, // transition 1538
  {2014866000U, -18000}, // transition 1539
  {2025752400U, -14400}, // transition 1540
  {2046315600U, -18000}, // transition 1541
  {2057202000U, -14400}, // transition 1542
  {2077765200U, -18000}, // transition 1543
  {2088651600U, -14400}, // transition 1544
  {2109214800U, -18000}, // transition 1545
  {2120101200U, -14400}, // transition 1546
  {2140664400U, -18000}, // transition 1547
  {2152155600U, -14400}, // transition 1548
  {2172718800U, -18000}, // transition 1549
  {2183605200U, -14400}, // transition 1550
  {2204168400U, -18000}, // transition 1551
  {2215054800U, -14400}, // transition 1552
  {2235618000U, -18000}, // transition 1553
  {2246504400U, -14400}, // transition 1554
  {2267067600U, -18000}, // transition 1555
  {2277954000U, -14400}, // transition 1556
  {2298517200U, -18000}, // transition 1557
  {2309403600U, -14400}, // transition 1558
  {2329966800U, -18000}, // transition 1559
  {2341458000U, -14400}, // transition 1560
  {2362021200U, -18000}, // transition 1561
  {2372907600U, -14400}, // transition 1562
  {2393470800U, -18000}, // transition 1563
  {2404357200U, -14400}, // transition 1564
  {2424920400U, -18000}, // transition 1565
  {2435806800U, -14400}, // transition 1566
  {2456370000U, -18000}, // transition 1567
  {2467256400U, -14400}, // transition 1568
  {2487819600U, -18000}, // transition 1569
  {2499310800U, -14400}, // transition 1570
  {2519874000U, -18000}, // transition 1571
  {2530760400U, -14400}, // transition 1572
  {2551323600U, -18000}, // transition 1573
  {2562210000U, -14400}, // transition 1574
  {2582773200U, -18000}, // transition 1575
  {2593659600U, -14400}, // transition 1576
  {2614222800U, -18000}, // transition 1577
  {2625109200U, -14400}, // transition 1578
  {2645672400U, -18000}, // transition 1579
  {2656558800U, -14400}, // transition 1580
  {2677122000U, -18000}, // transition 1581
  {2688613200U, -14400}, // transition 1582
  {2709176400U, -18000}, // transition 1583
  {2720062800U, -14400}, // transition 1584
  {2740626000U, -18000}, // transition 1585
  {2751512400U, -14400}, // transition 1586
  {2772075600U, -18000}, // transition 1587
  {2782962000U, -14400}, // transition 1588
  {2803525200U, -18000}, // transition 1589
  {2814411600U, -14400}, // transition 1590
  {2834974800U, -18000}, // transition 1591
  {2846466000U, -14400}, // transition 1592
  {2867029200U, -18000}, // transition 1593
  {2877915600U, -14400}, // transition 1594
  {2898478800U, -18000}, // transition 1595
  {2909365200U, -14400}, // transition 1596
  {2929928400U, -18000}, // transition 1597
  {2940814800U, -14400}, // transition 1598
  {2961378000U, -18000}, // transition 1599
  {2972264400U, -14400}, // transition 1600
  {2992827600U, -18000}, // transition 1601
  {3003714000U, -14400}, // transition 1602
  {3024277200U, -18000}, // transition 1603
  {3035768400U, -14400}, // transition 1604
  {3056331600U, -18000}, // transition 1605
  {3067218000U, -14400}, // transition 1606
  {3087781200U, -18000}, // transition 1607
  {3098667600U, -14400}, // transition 1608
  {3119230800U, -18000}, // transition 1609
  {3130117200U, -14400}, // transition 1610
  {3150680400U, -18000}, // transition 1611
  {3161566800U, -14400}, // transition 1612
  {3182130000U, -18000}, // transition 1613
  {3193016400U, -14400}, // transition 1614
  {3213579600U, -18000}, // transition 1615
  {3225070800U, -14400}, // transition 1616
  {3245634000U, -18000}, // transition 1617
  {3256520400U, -14400}, // transition 1618
  {3277083600U, -18000}, // transition 1619
  {3287970000U, -14400}, // transition 1620
  {3308533200U, -18000}, // transition 1621
  {3319419600U, -14400}, // transition 1622
  {3339982800U, -18000}, // transition 1623
  {3350869200U, -14400}, // transition 1624
  {3371432400U, -18000}, // transition 1625
  {3382923600U, -14400}, // transition 1626
  {3403486800U, -18000}, // transition 1627
  {3414373200U, -14400}, // transition 1628
  {3434936400U, -18000}, // transition 1629
  {3445822800U, -14400}, // transition 1630
  {3466386000U, -18000}, // transition 1631
  {3477272400U, -14400}, // transition 1632
  {3497835600U, -18000}, // transition 1633
  {3508722000U, -14400}, // transition 1634
  {3529285200U, -18000}, // transition 1635
  {3540171600U, -14400}, // transition 1636
  {3560734800U, -18000}, // transition 1637
  {3572226000U, -14400}, // transition 1638
  {3592789200U, -18000}, // transition 1639
  {3603675600U, -14400}, // transition 1640
  {3624238800U, -18000}, // transition 1641
  {3635125200U, -14400}, // transition 1642
  {3655688400U, -18000}, // transition 1643
  {3666574800U, -14400}, // transition 1644
  {3687138000U, -18000}, // transition 1645
  {3698024400U, -14400}, // transition 1646
  {3718587600U, -18000}, // transition 1647
  {3730078800U, -14400}, // transition 1648
  {3750642000U, -18000}, // transition 1649
  {3761528400U, -14400}, // transition 1650
  {3782091600U, -18000}, // transition 1651
  {3792978000U, -14400}, // transition 1652
  {3813541200U, -18000}, // transition 1653
  {3824427600U, -14400}, // transition 1654
  {3844990800U, -18000}, // transition 1655
  {3855877200U, -14400}, // transition 1656
  {3876440400U, -18000}, // transition 1657
  {3887326800U, -14400}, // transition 1658
  {3907890000U, -18000}, // transition 1659
  {3919381200U, -14400}, // transition 1660
  {3939944400U, -18000}, // transition 1661
  {3950830800U, -14400}, // transition 1662
  {3971394000U, -18000}, // transition 1663
  {3982280400U, -14400}, // transition 1664
  {4002843600U, -18000}, // transition 1665
  {4013730000U, -14400}, // transition 1666
  {4034293200U, -18000}, // transition 1667
  {4045179600U, -14400}, // transition 1668
  {4065742800U, -18000}, // transition 1669
  {4076629200U, -14400}, // transition 1670
  {4097192400U, -18000}, // transition 1671
  {4108683600U, -14400}, // transition 1672
  {4129246800U, -18000}, // transition 1673
  {1767225600U, -10800}, // transition 1674
  {1772946000U, -7200}, // transition 1675
  {1793505600U, -10800}, // transition 1676
  {1805000400U, -7200}, // transition 1677
  {1825560000U, -10800}, // transition 1678
  {1836450000U, -7200}, // transition 1679
  {1857009600U, -10800}, // transition 1680
  {1867899600U, -7200}, // transition 1681
  {1888459200U, -10800}, // transition 1682
  {1899349200U, -7200}, // transition 1683
  {1919908800U, -10800}, // transition 1684
  {1930798800U, -7200}, // transition 1685
  {1951358400U, -10800}, // transition 1686
  {1962853200U, -7200}, // transition 1687
  {1983412800U, -10800}, // transition 1688
  {1994302800U, -7200}, // transition 1689
  {2014862400U, -10800}, // transition 1690
  {2025752400U, -7200}, // transition 1691
  {2046312000U, -10800}, // transition 1692
  {2057202000U, -7200}, // transition 1693
  {2077761600U, -10800}, // transition 1694
  {2088651600U, -7200}, // transition 1695
  {2109211200U, -10800}, // transition 1696
  {2120101200U, -7200}, // transition 1697
  {2140660800U, -10800}, // transition 1698
  {2152155600U, -7200}, // transition 1699
  {2172715200U, -10800}, // transition 1700
  {2183605200U, -7200}, // transition 1701
  {2204164800U, -10800}, // transition 1702
  {2215054800U, -7200}, // transition 1703
  {2235614400U, -10800}, // transition 1704
  {2246504400U, -7200}, // transition 1705
  {2267064000U, -10800}, // transition 1706
  {2277954000U, -7200}, // transition 1707
  {2298513600U, -10800}, // transition 1708
  {2309403600U, -7200}, // transition 1709
  {2329963200U, -10800}, // transition 1710
  {2341458000U, -7200}, // transition 1711
  {2362017600U, -10800}, // transition 1712
  {2372907600U, -7200}, // transition 1713
  {2393467200U, -10800}, // transition 1714
  {2404357200U, -7200}, // transition 1715
  {2424916800U, -10800}, // transition 1716
  {2435806800U, -7200}, // transition 1717
  {2456366400U, -10800}, // transition 1718
  {2467256400U, -7200}, // transition 1719
  {2487816000U, -10800}, // transition 1720
  {2499310800U, -7200}, // transition 1721
  {2519870400U, -10800}, // transition 1722
  {2530760400U, -7200}, // transition 1723
  {2551320000U, -10800}, // transition 1724
  {2562210000U, -7200}, // transition 1725
  {2582769600U, -10800}, // transition 1726
  {2593659600U, -7200}, // transition 1727
  {2614219200U, -10800}, // transition 1728
  {2625109200U, -7200}, // transition 1729
  {2645668800U, -10800}, // transition 1730
  {2656558800U, -7200}, // transition 1731
  {2677118400U, -10800}, // transition 1732
  {2688613200U, -7200}, // transition 1733
  {2709172800U, -10800}, // transition 1734
  {2720062800U, -7200}, // transition 1735
  {2740622400U, -10800}, // transition 1736
  {2751512400U, -7200}, // transition 1737
  {2772072000U, -10800}, // transition 1738
  {2782962000U, -7200}, // transition 1739
  {2803521600U, -10800}, // transition 1740
  {2814411600U, -7200}, // transition 1741
  {2834971200U, -10800}, // transition 1742
  {2846466000U, -7200}, // transition 1743
  {2867025600U, -10800}, // transition 1744
  {2877915600U, -7200}, // transition 1745
  {2898475200U, -10800}, // transition 1746
  {2909365200U, -7200}, // transition 1747
  {2929924800U, -10800}, // transition 1748
  {2940814800U, -7200}, // transition 1749
  {2961374400U, -10800}, // transition 1750
  {2972264400U, -7200}, // transition 1751
  {2992824000U, -10800}, // transition 1752
  {3003714000U, -7200}, // transition 1753
  {3024273600U, -10800}, // transition 1754
  {3035768400U, -7200}, // transition 1755
  {3056328000U, -10800}, // transition 1756
  {3067218000U, -7200}, // transition 1757
  {3087777600U, -10800}, // transition 1758
  {3098667600U, -7200}, // transition 1759
  {3119227200U, -10800}, // transition 1760
  {3130117200U, -7200}, // transition 1761
  {3150676800U, -10800}, // transition 1762
  {3161566800U, -7200}, // transition 1763
  {3182126400U, -10800}, // transition 1764
  {3193016400U, -7200}, // transition 1765
  {3213576000U, -10800}, // transition 1766
  {3225070800U, -7200}, // transition 1767
  {3245630400U, -10800}, // transition 1768
  {3256520400U, -7200}, // transition 1769
  {3277080000U, -10800}, // transition 1770
  {3287970000U, -7200}, // transition 1771
  {3308529600U, -10800}, // transition 1772
  {3319419600U, -7200}, // transition 1773
  {3339979200U, -10800}, // transition 1774
  {3350869200U, -7200}, // transition 1775
  {3371428800U, -10800}, // transition 1776
  {3382923600U, -7200}, // transition 1777
  {3403483200U, -10800}, // transition 1778
  {3414373200U, -7200}, // transition 1779
  {3434932800U, -10800}, // transition 1780
  {3445822800U, -7200}, // transition 1781
  {3466382400U, -10800}, // transition 1782
  {3477272400U, -7200}, // transition 1783
  {3497832000U, -10800}, // transition 1784
  {3508722000U, -7200}, // transition 1785
  {3529281600U, -10800}, // transition 1786
  {3540171600U, -7200}, // transition 1787
  {3560731200U, -10800}, // transition 1788
  {3572226000U, -7200}, // transition 1789
  {3592785600U, -10800}, // transition 1790
  {3603675600U, -7200}, // transition 1791
  {3624235200U, -10800}, // transition 1792
  {3635125200U, -7200}, // transition 1793
  {3655684800U, -10800}, // transition 1794
  {3666574800U, -7200}, // transition 1795
  {3687134400U, -10800}, // transition 1796
  {3698024400U, -7200}, // transition 1797
  {3718584000U, -10800}, // transition 1798
  {3730078800U, -7200}, // transition 1799
  {3750638400U, -10800}, // transition 1800
  {3761528400U, -7200}, // transition 1801
  {3782088000U, -10800}, // transition 1802
  {3792978000U, -7200}, // transition 1803
  {3813537600U, -10800}, // transition 1804
  {3824427600U, -7200}, // transition 1805
  {3844987200U, -10800}, // transition 1806
  {3855877200U, -7200}, // transition 1807
  {3876436800U, -10800}, // transition 1808
  {3887326800U, -7200}, // transition 1809
  {3907886400U, -10800}, // transition 1810
  {3919381200U, -7200}, // transition 1811
  {3939940800U, -10800}, // transition 1812
  {3950830800U, -7200}, // transition 1813
  {3971390400U, -10800}, // transition 1814
  {3982280400U, -7200}, // transition 1815
  {4002840000U, -10800}, // transition 1816
  {4013730000U, -7200}, // transition 1817
  {4034289600U, -10800}, // transition 1818
  {4045179600U, -7200}, // transition 1819
  {4065739200U, -10800}, // transition 1820
  {4076629200U, -7200}, // transition 1821
  {4097188800U, -10800}, // transition 1822
  {4108683600U, -7200}, // transition 1823
  {4129243200U, -10800}, // transition 1824
  {1767225600U, -7200}, // transition 1825
  {1767225600U, -21600}, // transition 1826
  {1772956800U, -18000}, // transition 1827
  {1767225600U, -10800}, // transition 1828
  {1775358000U, -14400}, // transition 1829
  {1788667200U, -10800}, // transition 1830
  {1806807600U, -14400}, // transition 1831
  {1820116800U, -10800}, // transition 1832
  {1838257200U, -14400}, // transition 1833
  {1851566400U, -10800}, // transition 1834
  {1870311600U, -14400}, // transition 1835
  {1883016000U, -10800}, // transition 1836
  {1901761200U, -14400}, // transition 1837
  {1915070400U, -10800}, // transition 1838
  {1933210800U, -14400}, // transition 1839
  {1946520000U, -10800}, // transition 1840
  {1964660400U, -14400}, // transition 1841
  {1977969600U, -10800}, // transition 1842
  {1996110000U, -14400}, // transition 1843
  {2009419200U, -10800}, // transition 1844
  {2027559600U, -14400}, // transition 1845
  {2040868800U, -10800}, // transition 1846
  {2059614000U, -14400}, // transition 1847
  {2072318400U, -10800}, // transition 1848
  {2091063600U, -14400}, // transition 1849
  {2104372800U, -10800}, // transition 1850
  {2122513200U, -14400}, // transition 1851
  {2135822400U, -10800}, // transition 1852
  {2153962800U, -14400}, // transition 1853
  {2167272000U, -10800}, // transition 1854
  {2185412400U, -14400}, // transition 1855
  {2198721600U, -10800}, // transition 1856
  {2217466800U, -14400}, // transition 1857
  {2230171200U, -10800}, // transition 1858
  {2248916400U, -14400}, // transition 1859
  {2262225600U, -10800}, // transition 1860
  {2280366000U, -14400}, // transition 1861
  {2293675200U, -10800}, // transition 1862
  {2311815600U, -14400}, // transition 1863
  {2325124800U, -10800}, // transition 1864
  {2343265200U, -14400}, // transition 1865
  {2356574400U, -10800}, // transition 1866
  {2374714800U, -14400}, // transition 1867
  {2388024000U, -10800}, // transition 1868
  {2406769200U, -14400}, // transition 1869
  {2419473600U, -10800}, // transition 1870
  {2438218800U, -14400}, // transition 1871
  {2451528000U, -10800}, // transition 1872
  {2469668400U, -14400}, // transition 1873
  {2482977600U, -10800}, // transition 1874
  {2501118000U, -14400}, // transition 1875
  {2514427200U, -10800}, // transition 1876
  {2532567600U, -14400}, // transition 1877
  {2545876800U, -10800}, // transition 1878
  {2564017200U, -14400}, // transition 1879
  {2577326400U, -10800}, // transition 1880
  {2596071600U, -14400}, // transition 1881
  {2609380800U, -10800}, // transition 1882
  {2627521200U, -14400}, // transition 1883
  {2640830400U, -10800}, // transition 1884
  {2658970800U, -14400}, // transition 1885
  {2672280000U, -10800}, // transition 1886
  {2690420400U, -14400}, // transition 1887
  {2703729600U, -10800}, // transition 1888
  {2721870000U, -14400}, // transition 1889
  {2735179200U, -10800}, // transition 1890
  {2753924400U, -14400}, // transition 1891
  {2766628800U, -10800}, // transition 1892
  {2785374000U, -14400}, // transition 1893
  {2798683200U, -10800}, // transition 1894
  {2816823600U, -14400}, // transition 1895
  {2830132800U, -10800}, // transition 1896
  {2848273200U, -14400}, // transition 1897
  {2861582400U, -10800}, // transition 1898
  {2879722800U, -14400}, // transition 1899
  {2893032000U, -10800}, // transition 1900
  {2911172400U, -14400}, // transition 1901
  {2924481600U, -10800}, // transition 1902
  {2943226800U, -14400}, // transition 1903
  {2955931200U, -10800}, // transition 1904
  {2974676400U, -14400}, // transition 1905
  {2987985600U, -10800}, // transition 1906
  {3006126000U, -14400}, // transition 1907
  {3019435200U, -10800}, // transition 1908
  {3037575600U, -14400}, // transition 1909
  {3050884800U, -10800}, // transition 1910
  {3069025200U, -14400}, // transition 1911
  {3082334400U, -10800}, // transition 1912
  {3101079600U, -14400}, // transition 1913
  {3113784000U, -10800}, // transition 1914
  {3132529200U, -14400}, // transition 1915
  {3145838400U, -10800}, // transition 1916
  {3163978800U, -14400}, // transition 1917
  {3177288000U, -10800}, // transition 1918
  {3195428400U, -14400}, // transition 1919
  {3208737600U, -10800}, // transition 1920
  {3226878000U, -14400}, // transition 1921
  {3240187200U, -10800}, // transition 1922
  {3258327600U, -14400}, // transition 1923
  {3271636800U, -10800}, // transition 1924
  {3290382000U, -14400}, // transition 1925
  {3303086400U, -10800}, // transition 1926
  {3321831600U, -14400}, // transition 1927
  {3335140800U, -10800}, // transition 1928
  {3353281200U, -14400}, // transition 1929
  {3366590400U, -10800}, // transition 1930
  {3384730800U, -14400}, // transition 1931
  {3398040000U, -10800}, // transition 1932
  {3416180400U, -14400}, // transition 1933
  {3429489600U, -10800}, // transition 1934
  {3447630000U, -14400}, // transition 1935
  {3460939200U, -10800}, // transition 1936
  {3479684400U, -14400}, // transition 1937
  {3492993600U, -10800}, // transition 1938
  {3511134000U, -14400}, // transition 1939
  {3524443200U, -10800}, // transition 1940
  {3542583600U, -14400}, // transition 1941
  {3555892800U, -10800}, // transition 1942
  {3574033200U, -14400}, // transition 1943
  {3587342400U, -10800}, // transition 1944
  {3605482800U, -14400}, // transition 1945
  {3618792000U, -10800}, // transition 1946
  {3637537200U, -14400}, // transition 1947
  {3650241600U, -10800}, // transition 1948
  {3668986800U, -14400}, // transition 1949
  {3682296000U, -10800}, // transition 1950
  {3700436400U, -14400}, // transition 1951
  {3713745600U, -10800}, // transition 1952
  {3731886000U, -14400}, // transition 1953
  {3745195200U, -10800}, // transition 1954
  {3763335600U, -14400}, // transition 1955
  {3776644800U, -10800}, // transition 1956
  {3794785200U, -14400}, // transition 1957
  {3808094400U, -10800}, // transition 1958
  {3826839600U, -14400}, // transition 1959
  {3839544000U, -10800}, // transition 1960
  {3858289200U, -14400}, // transition 1961
  {3871598400U, -10800}, // transition 1962
  {3889738800U, -14400}, // transition 1963
  {3903048000U, -10800}, // transition 1964
  {3921188400U, -14400}, // transition 1965
  {3934497600U, -10800}, // transition 1966
  {3952638000U, -14400}, // transition 1967
  {3965947200U, -10800}, // transition 1968
  {3984692400U, -14400}, // transition 1969
  {3997396800U, -10800}, // transition 1970
  {4016142000U, -14400}, // transition 1971
  {4029451200U, -10800}, // transition 1972
  {4047591600U, -14400}, // transition 1973
  {4060900800U, -10800}, // transition 1974
  {4079041200U, -14400}, // transition 1975
  {4092350400U, -10800}, // transition 1976
  {4110490800U, -14400}, // transition 1977
  {4123800000U, -10800}, // transition 1978
  {1767225600U, -12600}, // transition 1979
  {1772947800U, -9000}, // transition 1980
  {1793507400U, -12600}, // transition 1981
  {1805002200U, -9000}, // transition 1982
  {1825561800U, -12600}, // transition 1983
  {1836451800U, -9000}, // transition 1984
  {1857011400U, -12600}, // transition 1985
  {1867901400U, -9000}, // transition 1986
  {1888461000U, -12600}, // transition 1987
  {1899351000U, -9000}, // transition 1988
  {1919910600U, -12600}, // transition 1989
  {1930800600U, -9000}, // transition 1990
  {1951360200U, -12600}, // transition 1991
  {1962855000U, -9000}, // transition 1992
  {1983414600U, -12600}, // transition 1993
  {1994304600U, -9000}, // transition 1994
  {2014864200U, -12600}, // transition 1995
  {2025754200U, -9000}, // transition 1996
  {2046313800U, -12600}, // transition 1997
  {2057203800U, -9000}, // transition 1998
  {2077763400U, -12600}, // transition 1999
  {2088653400U, -9000}, // transition 2000
  {2109213000U, -12600}, // transition 2001
  {2120103000U, -9000}, // transition 2002
  {2140662600U, -12600}, // transition 2003
  {2152157400U, -9000}, // transition 2004
  {2172717000U, -12600}, // transition 2005
  {2183607000U, -9000}, // transition 2006
  {2204166600U, -12600}, // transition 2007
  {2215056600U, -9000}, // transition 2008
  {2235616200U, -12600}, // transition 2009
  {2246506200U, -9000}, // transition 2010
  {2267065800U, -12600}, // transition 2011
  {2277955800U, -9000}, // transition 2012
  {2298515400U, -12600}, // transition 2013
  {2309405400U, -9000}, // transition 2014
  {2329965000U, -12600}, // transition 2015
  {2341459800U, -9000}, // transition 2016
  {2362019400U, -12600}, // transition 2017
  {2372909400U, -9000}, // transition 2018
  {2393469000U, -12600}, // transition 2019
  {2404359000U, -9000}, // transition 2020
  {2424918600U, -12600}, // transition 2021
  {2435808600U, -9000}, // transition 2022
  {2456368200U, -12600}, // transition 2023
  {2467258200U, -9000}, // transition 2024
  {2487817800U, -12600}, // transition 2025
  {2499312600U, -9000}, // transition 2026
  {2519872200U, -12600}, // transition 2027
  {2530762200U, -9000}, // transition 2028
  {2551321800U, -12600}, // transition 2029
  {2562211800U, -9000}, // transition 2030
  {2582771400U, -12600}, // transition 2031
  {2593661400U, -9000}, // transition 2032
  {2614221000U, -12600}, // transition 2033
  {2625111000U, -9000}, // transition 2034
  {2645670600U, -12600}, // transition 2035
  {2656560600U, -9000}, // transition 2036
  {2677120200U, -12600}, // transition 2037
  {2688615000U, -9000}, // transition 2038
  {2709174600U, -12600}, // transition 2039
  {2720064600U, -9000}, // transition 2040
  {2740624200U, -12600}, // transition 2041
  {2751514200U, -9000}, // transition 2042
  {2772073800U, -12600}, // transition 2043
  {2782963800U, -9000}, // transition 2044
  {2803523400U, -12600}, // transition 2045
  {2814413400U, -9000}, // transition 2046
  {2834973000U, -12600}, // transition 2047
  {2846467800U, -9000}, // transition 2048
  {2867027400U, -12600}, // transition 2049
  {2877917400U, -9000}, // transition 2050
  {2898477000U, -12600}, // transition 2051
  {2909367000U, -9000}, // transition 2052
  {2929926600U, -12600}, // transition 2053
  {2940816600U, -9000}, // transition 2054
  {2961376200U, -12600}, // transition 2055
  {2972266200U, -9000}, // transition 2056
  {2992825800U, -12600}, // transition 2057
  {3003715800U, -9000}, // transition 2058
  {3024275400U, -12600}, // transition 2059
  {3035770200U, -9000}, // transition 2060
  {3056329800U, -12600}, // transition 2061
  {3067219800U, -9000}, // transition 2062
  {3087779400U, -12600}, // transition 2063
  {3098669400U, -9000}, // transition 2064
  {3119229000U, -12600}, // transition 2065
  {3130119000U, -9000}, // transition 2066
  {3150678600U, -12600}, // transition 2067
  {3161568600U, -9000}, // transition 2068
  {3182128200U, -12600}, // transition 2069
  {3193018200U, -9000}, // transition 2070
  {3213577800U, -12600}, // transition 2071
  {3225072600U, -9000}, // transition 2072
  {3245632200U, -12600}, // transition 2073
  {3256522200U, -9000}, // transition 2074
  {3277081800U, -12600}, // transition 2075
  {3287971800U, -9000}, // transition 2076
  {3308531400U, -12600}, // transition 2077
  {3319421400U, -9000}, // transition 2078
  {3339981000U, -12600}, // transition 2079
  {3350871000U, -9000}, // transition 2080
  {3371430600U, -12600}, // transition 2081
  {3382925400U, -9000}, // transition 2082
  {3403485000U, -12600}, // transition 2083
  {3414375000U, -9000}, // transition 2084
  {3434934600U, -12600}, // transition 2085
  {3445824600U, -9000}, // transition 2086
  {3466384200U, -12600}, // transition 2087
  {3477274200U, -9000}, // transition 2088
  {3497833800U, -12600}, // transition 2089
  {3508723800U, -9000}, // transition 2090
  {3529283400U, -12600}, // transition 2091
  {3540173400U, -9000}, // transition 2092
  {3560733000U, -12600}, // transition 2093
  {3572227800U, -9000}, // transition 2094
  {3592787400U, -12600}, // transition 2095
  {3603677400U, -9000}, // transition 2096
  {3624237000U, -12600}, // transition 2097
  {3635127000U, -9000}, // transition 2098
  {3655686600U, -12600}, // transition 2099
  {3666576600U, -9000}, // transition 2100
  {3687136200U, -12600}, // transition 2101
  {3698026200U, -9000}, // transition 2102
  {3718585800U, -12600}, // transition 2103
  {3730080600U, -9000}, // transition 2104
  {3750640200U, -12600}, // transition 2105
  {3761530200U, -9000}, // transition 2106
  {3782089800U, -12600}, // transition 2107
  {3792979800U, -9000}, // transition 2108
  {3813539400U, -12600}, // transition 2109
  {3824429400U, -9000}, // transition 2110
  {3844989000U, -12600}, // transition 2111
  {3855879000U, -9000}, // transition 2112
  {3876438600U, -12600}, // transition 2113
  {3887328600U, -9000}, // transition 2114
  {3907888200U, -12600}, // transition 2115
  {3919383000U, -9000}, // transition 2116
  {3939942600U, -12600}, // transition 2117
  {3950832600U, -9000}, // transition 2118
  {3971392200U, -12600}, // transition 2119
  {3982282200U, -9000}, // transition 2120
  {4002841800U, -12600}, // transition 2121
  {4013731800U, -9000}, // transition 2122
  {4034291400U, -12600}, // transition 2123
  {4045181400U, -9000}, // transition 2124
  {4065741000U, -12600}, // transition 2125
  {4076631000U, -9000}, // transition 2126
  {4097190600U, -12600}, // transition 2127
  {4108685400U, -9000}, // transition 2128
  {4129245000U, -12600}, // transition 2129
  {1767225600U, -28800}, // transition 2130
  {1772964000U, -25200}, // transition 2131
  {1767225600U, 28800}, // transition 2132
  {1767225600U, 25200}, // transition 2133
  {1767225600U, 36000}, // transition 2134
  {1767225600U, 39600}, // transition 2135
  {1775318400U, 36000}, // transition 2136
  {1791043200U, 39600}, // transition 2137
  {1806768000U, 36000}, // transition 2138
  {1822492800U, 39600}, // transition 2139
  {1838217600U, 36000}, // transition 2140
  {1853942400U, 39600}, // transition 2141
  {1869667200U, 36000}, // transition 2142
  {1885996800U, 39600}, // transition 2143
  {1901721600U, 36000}, // transition 2144
  {1917446400U, 39600}, // transition 2145
  {1933171200U, 36000}, // transition 2146
  {1948896000U, 39600}, // transition 2147
  {1964620800U, 36000}, // transition 2148
  {1980345600U, 39600}, // transition 2149
  {1996070400U, 36000}, // transition 2150
  {2011795200U, 39600}, // transition 2151
  {2027520000U, 36000}, // transition 2152
  {2043244800U, 39600}, // transition 2153
  {2058969600U, 36000}, // transition 2154
  {2075299200U, 39600}, // transition 2155
  {2091024000U, 36000}, // transition 2156
  {2106748800U, 39600}, // transition 2157
  {2122473600U, 36000}, // transition 2158
  {2138198400U, 39600}, // transition 2159
  {2153923200U, 36000}, // transition 2160
  {2169648000U, 39600}, // transition 2161
  {2185372800U, 36000}, // transition 2162
  {2201097600U, 39600}, // transition 2163
  {2216822400U, 36000}, // transition 2164
  {2233152000U, 39600}, // transition 2165
  {2248876800U, 36000}, // transition 2166
  {2264601600U, 39600}, // transition 2167
  {2280326400U, 36000}, // transition 2168
  {2296051200U, 39600}, // transition 2169
  {2311776000U, 36000}, // transition 2170
  {2327500800U, 39600}, // transition 2171
  {2343225600U, 36000}, // transition 2172
  {2358950400U, 39600}, // transition 2173
  {2374675200U, 36000}, // transition 2174
  {2390400000U, 39600}, // transition 2175
  {2406124800U, 36000}, // transition 2176
  {2422454400U, 39600}, // transition 2177
  {2438179200U, 36000}, // transition 2178
  {2453904000U, 39600}, // transition 2179
  {2469628800U, 36000}, // transition 2180
  {2485353600U, 39600}, // transition 2181
  {2501078400U, 36000}, // transition 2182
  {2516803200U, 39600}, // transition 2183
  {2532528000U, 36000}, // transition 2184
  {2548252800U, 39600}, // transition 2185
  {2563977600U, 36000}, // transition 2186
  {2579702400U, 39600}, // transition 2187
  {2596032000U, 36000}, // transition 2188
  {2611756800U, 39600}, // transition 2189
  {2627481600U, 36000}, // transition 2190
  {2643206400U, 39600}, // transition 2191
  {2658931200U, 36000}, // transition 2192
  {2674656000U, 39600}, // transition 2193
  {2690380800U, 36000}, // transition 2194
  {2706105600U, 39600}, // transition 2195
  {2721830400U, 36000}, // transition 2196
  {2737555200U, 39600}, // transition 2197
  {2753280000U, 36000}, // transition 2198
  {2769609600U, 39600}, // transition 2199
  {2785334400U, 36000}, // transition 2200
  {2801059200U, 39600}, // transition 2201
  {2816784000U, 36000}, // transition 2202
  {2832508800U, 39600}, // transition 2203
  {2848233600U, 36000}, // transition 2204
  {2863958400U, 39600}, // transition 2205
  {2879683200U, 36000}, // transition 2206
  {2895408000U, 39600}, // transition 2207
  {2911132800U, 36000}, // transition 2208
  {2926857600U, 39600}, // transition 2209
  {2942582400U, 36000}, // transition 2210
  {2958912000U, 39600}, // transition 2211
  {2974636800U, 36000}, // transition 2212
  {2990361600U, 39600}, // transition 2213
  {3006086400U, 36000}, // transition 2214
  {3021811200U, 39600}, // transition 2215
  {3037536000U, 36000}, // transition 2216
  {3053260800U, 39600}, // transition 2217
  {3068985600U, 36000}, // transition 2218
  {3084710400U, 39600}, // transition 2219
  {3100435200U, 36000}, // transition 2220
  {3116764800U, 39600}, // transition 2221
  {3132489600U, 36000}, // transition 2222
  {3148214400U, 39600}, // transition 2223
  {3163939200U, 36000}, // transition 2224
  {3179664000U, 39600}, // transition 2225
  {3195388800U, 36000}, // transition 2226
  {3211113600U, 39600}, // transition 2227
  {3226838400U, 36000}, // transition 2228
  {3242563200U, 39600}, // transition 2229
  {3258288000U, 36000}, // transition 2230
  {3274012800U, 39600}, // transition 2231
  {3289737600U, 36000}, // transition 2232
  {3306067200U, 39600}, // transition 2233
  {3321792000U, 36000}, // transition 2234
  {3337516800U, 39600}, // transition 2235
  {3353241600U, 36000}, // transition 2236
  {3368966400U, 39600}, // transition 2237
  {3384691200U, 36000}, // transition 2238
  {3400416000U, 39600}, // transition 2239
  {3416140800U, 36000}, // transition 2240
  {3431865600U, 39600}, // transition 2241
  {3447590400U, 36000}, // transition 2242
  {3463315200U, 39600}, // transition 2243
  {3479644800U, 36000}, // transition 2244
  {3495369600U, 39600}, // transition 2245
  {3511094400U, 36000}, // transition 2246
  {3526819200U, 39600}, // transition 2247
  {3542544000U, 36000}, // transition 2248
  {3558268800U, 39600}, // transition 2249
  {3573993600U, 36000}, // transition 2250
  {3589718400U, 39600}, // transition 2251
  {3605443200U, 36000}, // transition 2252
  {3621168000U, 39600}, // transition 2253
  {3636892800U, 36000}, // transition 2254
  {3653222400U, 39600}, // transition 2255
  {3668947200U, 36000}, // transition 2256
  {3684672000U, 39600}, // transition 2257
  {3700396800U, 36000}, // transition 2258
  {3716121600U, 39600}, // transition 2259
  {3731846400U, 36000}, // transition 2260
  {3747571200U, 39600}, // transition 2261
  {3763296000U, 36000}, // transition 2262
  {3779020800U, 39600}, // transition 2263
  {3794745600U, 36000}, // transition 2264
  {3810470400U, 39600}, // transition 2265
  {3826195200U, 36000}, // transition 2266
  {3842524800U, 39600}, // transition 2267
  {3858249600U, 36000}, // transition 2268
  {3873974400U, 39600}, // transition 2269
  {3889699200U, 36000}, // transition 2270
  {3905424000U, 39600}, // transition 2271
  {3921148800U, 36000}, // transition 2272
  {3936873600U, 39600}, // transition 2273
  {3952598400U, 36000}, // transition 2274
  {3968323200U, 39600}, // transition 2275
  {3984048000U, 36000}, // transition 2276
  {4000377600U, 39600}, // transition 2277
  {4016102400U, 36000}, // transition 2278
  {4031827200U, 39600}, // transition 2279
  {4047552000U, 36000}, // transition 2280
  {4063276800U, 39600}, // transition 2281
  {4079001600U, 36000}, // transition 2282
  {4094726400U, 39600}, // transition 2283
  {4110451200U, 36000}, // transition 2284
  {4126176000U, 39600}, // transition 2285
  {1767225600U, 18000}, // transition 2286
  {1767225600U, 46800}, // transition 2287
  {1775311200U, 43200}, // transition 2288
  {1790431200U, 46800}, // transition 2289
  {1806760800U, 43200}, // transition 2290
  {1821880800U, 46800}, // transition 2291
  {1838210400U, 43200}, // transition 2292
  {1853330400U, 46800}, // transition 2293
  {1869660000U, 43200}, // transition 2294
  {1885384800U, 46800}, // transition 2295
  {1901714400U, 43200}, // transition 2296
  {1916834400U, 46800}, // transition 2297
  {1933164000U, 43200}, // transition 2298
  {1948284000U, 46800}, // transition 2299
  {1964613600U, 43200}, // transition 2300
  {1979733600U, 46800}, // transition 2301
  {1996063200U, 43200}, // transition 2302
  {2011183200U, 46800}, // transition 2303
  {2027512800U, 43200}, // transition 2304
  {2042632800U, 46800}, // transition 2305
  {2058962400U, 43200}, // transition 2306
  {2074687200U, 46800}, // transition 2307
  {2091016800U, 43200}, // transition 2308
  {2106136800U, 46800}, // transition 2309
  {2122466400U, 43200}, // transition 2310
  {2137586400U, 46800}, // transition 2311
  {2153916000U, 43200}, // transition 2312
  {2169036000U, 46800}, // transition 2313
  {2185365600U, 43200}, // transition 2314
  {2200485600U, 46800}, // transition 2315
  {2216815200U, 43200}, // transition 2316
  {2232540000U, 46800}, // transition 2317
  {2248869600U, 43200}, // transition 2318
  {2263989600U, 46800}, // transition 2319
  {2280319200U, 43200}, // transition 2320
  {2295439200U, 46800}, // transition 2321
  {2311768800U, 43200}, // transition 2322
  {2326888800U, 46800}, // transition 2323
  {2343218400U, 43200}, // transition 2324
  {2358338400U, 46800}, // transition 2325
  {2374668000U, 43200}, // transition 2326
  {2389788000U, 46800}, // transition 2327
  {2406117600U, 43200}, // transition 2328
  {2421842400U, 46800}, // transition 2329
  {2438172000U, 43200}, // transition 2330
  {2453292000U, 46800}, // transition 2331
  {2469621600U, 43200}, // transition 2332
  {2484741600U, 46800}, // transition 2333
  {2501071200U, 43200}, // transition 2334
  {2516191200U, 46800}, // transition 2335
  {2532520800U, 43200}, // transition 2336
  {2547640800U, 46800}, // transition 2337
  {2563970400U, 43200}, // transition 2338
  {2579090400U, 46800}, // transition 2339
  {2596024800U, 43200}, // transition 2340
  {2611144800U, 46800}, // transition 2341
  {2627474400U, 43200}, // transition 2342
  {2642594400U, 46800}, // transition 2343
  {2658924000U, 43200}, // transition 2344
  {2674044000U, 46800}, // transition 2345
  {2690373600U, 43200}, // transition 2346
  {2705493600U, 46800}, // transition 2347
  {2721823200U, 43200}, // transition 2348
  {2736943200U, 46800}, // transition 2349
  {2753272800U, 43200}, // transition 2350
  {2768997600U, 46800}, // transition 2351
  {2785327200U, 43200}, // transition 2352
  {2800447200U, 46800}, // transition 2353
  {2816776800U, 43200}, // transition 2354
  {2831896800U, 46800}, // transition 2355
  {2848226400U, 43200}, // transition 2356
  {2863346400U, 46800}, // transition 2357
  {2879676000U, 43200}, // transition 2358
  {2894796000U, 46800}, // transition 2359
  {2911125600U, 43200}, // transition 2360
  {2926245600U, 46800}, // transition 2361
  {2942575200U, 43200}, // transition 2362
  {2958300000U, 46800}, // transition 2363
  {2974629600U, 43200}, // transition 2364
  {2989749600U, 46800}, // transition 2365
  {3006079200U, 43200}, // transition 2366
  {3021199200U, 46800}, // transition 2367
  {3037528800U, 43200}, // transition 2368
  {3052648800U, 46800}, // transition 2369
  {3068978400U, 43200}, // transition 2370
  {3084098400U, 46800}, // transition 2371
  {3100428000U, 43200}, // transition 2372
  {3116152800U, 46800}, // transition 2373
  {3132482400U, 43200}, // transition 2374
  {3147602400U, 46800}, // transition 2375
  {3163932000U, 43200}, // transition 2376
  {3179052000U, 46800}, // transition 2377
  {3195381600U, 43200}, // transition 2378
  {3210501600U, 46800}, // transition 2379
  {3226831200U, 43200}, // transition 2380
  {3241951200U, 46800}, // transition 2381
  {3258280800U, 43200}, // transition 2382
  {3273400800U, 46800}, // transition 2383
  {3289730400U, 43200}, // transition 2384
  {3305455200U, 46800}, // transition 2385
  {3321784800U, 43200}, // transition 2386
  {3336904800U, 46800}, // transition 2387
  {3353234400U, 43200}, // transition 2388
  {3368354400U, 46800}, // transition 2389
  {3384684000U, 43200}, // transition 2390
  {3399804000U, 46800}, // transition 2391
  {3416133600U, 43200}, // transition 2392
  {3431253600U, 46800}, // transition 2393
  {3447583200U, 43200}, // transition 2394
  {3462703200U, 46800}, // transition 2395
  {3479637600U, 43200}, // transition 2396
  {3494757600U, 46800}, // transition 2397
  {3511087200U, 43200}, // transition 2398
  {3526207200U, 46800}, // transition 2399
  {3542536800U, 43200}, // transition 2400
  {3557656800U, 46800}, // transition 2401
  {3573986400U, 43200}, // transition 2402
  {3589106400U, 46800}, // transition 2403
  {3605436000U, 43200}, // transition 2404
  {3620556000U, 46800}, // transition 2405
  {3636885600U, 43200}, // transition 2406
  {3652610400U, 46800}, // transition 2407
  {3668940000U, 43200}, // transition 2408
  {3684060000U, 46800}, // transition 2409
  {3700389600U, 43200}, // transition 2410
  {3715509600U, 46800}, // transition 2411
  {3731839200U, 43200}, // transition 2412
  {3746959200U, 46800}, // transition 2413
  {3763288800U, 43200}, // transition 2414
  {3778408800U, 46800}, // transition 2415
  {3794738400U, 43200}, // transition 2416
  {3809858400U, 46800}, // transition 2417
  {3826188000U, 43200}, // transition 2418
  {3841912800U, 46800}, // transition 2419
  {3858242400U, 43200}, // transition 2420
  {3873362400U, 46800}, // transition 2421
  {3889692000U, 43200}, // transition 2422
  {3904812000U, 46800}, // transition 2423
  {3921141600U, 43200}, // transition 2424
  {3936261600U, 46800}, // transition 2425
  {3952591200U, 43200}, // transition 2426
  {3967711200U, 46800}, // transition 2427
  {3984040800U, 43200}, // transition 2428
  {3999765600U, 46800}, // transition 2429
  {4016095200U, 43200}, // transition 2430
  {4031215200U, 46800}, // transition 2431
  {4047544800U, 43200}, // transition 2432
  {4062664800U, 46800}, // transition 2433
  {4078994400U, 43200}, // transition 2434
  {4094114400U, 46800}, // transition 2435
  {4110444000U, 43200}, // transition 2436
  {4125564000U, 46800}, // transition 2437
  {1767225600U, 0}, // transition 2438
  {1774746000U, 7200}, // transition 2439
  {1792890000U, 0}, // transition 2440
  {1806195600U, 7200}, // transition 2441
  {1824944400U, 0}, // transition 2442
  {1837645200U, 7200}, // transition 2443
  {1856394000U, 0}, // transition 2444
  {1869094800U, 7200}, // transition 2445
  {1887843600U, 0}, // transition 2446
  {1901149200U, 7200}, // transition 2447
  {1919293200U, 0}, // transition 2448
  {1932598800U, 7200}, // transition 2449
  {1950742800U, 0}, // transition 2450
  {1964048400U, 7200}, // transition 2451
  {1982797200U, 0}, // transition 2452
  {1995498000U, 7200}, // transition 2453
  {2014246800U, 0}, // transition 2454
  {2026947600U, 7200}, // transition 2455
  {2045696400U, 0}, // transition 2456
  {2058397200U, 7200}, // transition 2457
  {2077146000U, 0}, // transition 2458
  {2090451600U, 7200}, // transition 2459
  {2108595600U, 0}, // transition 2460
  {2121901200U, 7200}, // transition 2461
  {2140045200U, 0}, // transition 2462
  {2153350800U, 7200}, // transition 2463
  {2172099600U, 0}, // transition 2464
  {2184800400U, 7200}, // transition 2465
  {2203549200U, 0}, // transition 2466
  {2216250000U, 7200}, // transition 2467
  {2234998800U, 0}, // transition 2468
  {2248304400U, 7200}, // transition 2469
  {2266448400U, 0}, // transition 2470
  {2279754000U, 7200}, // transition 2471
  {2297898000U, 0}, // transition 2472
  {2311203600U, 7200}, // transition 2473
  {2329347600U, 0}, // transition 2474
  {2342653200U, 7200}, // transition 2475
  {2361402000U, 0}, // transition 2476
  {2374102800U, 7200}, // transition 2477
  {2392851600U, 0}, // transition 2478
  {2405552400U, 7200}, // transition 2479
  {2424301200U, 0}, // transition 2480
  {2437606800U, 7200}, // transition 2481
  {2455750800U, 0}, // transition 2482
  {2469056400U, 7200}, // transition 2483
  {2487200400U, 0}, // transition 2484
  {2500506000U, 7200}, // transition 2485
  {2519254800U, 0}, // transition 2486
  {2531955600U, 7200}, // transition 2487
  {2550704400U, 0}, // transition 2488
  {2563405200U, 7200}, // transition 2489
  {2582154000U, 0}, // transition 2490
  {2595459600U, 7200}, // transition 2491
  {2613603600U, 0}, // transition 2492
  {2626909200U, 7200}, // transition 2493
  {2645053200U, 0}, // transition 2494
  {2658358800U, 7200}, // transition 2495
  {2676502800U, 0}, // transition 2496
  {2689808400U, 7200}, // transition 2497
  {2708557200U, 0}, // transition 2498
  {2721258000U, 7200}, // transition 2499
  {2740006800U, 0}, // transition 2500
  {2752707600U, 7200}, // transition 2501
  {2771456400U, 0}, // transition 2502
  {2784762000U, 7200}, // transition 2503
  {2802906000U, 0}, // transition 2504
  {2816211600U, 7200}, // transition 2505
  {2834355600U, 0}, // transition 2506
  {2847661200U, 7200}, // transition 2507
  {2866410000U, 0}, // transition 2508
  {2879110800U, 7200}, // transition 2509
  {2897859600U, 0}, // transition 2510
  {2910560400U, 7200}, // transition 2511
  {2929309200U, 0}, // transition 2512
  {2942010000U, 7200}, // transition 2513
  {2960758800U, 0}, // transition 2514
  {2974064400U, 7200}, // transition 2515
  {2992208400U, 0}, // transition 2516
  {3005514000U, 7200}, // transition 2517
  {3023658000U, 0}, // transition 2518
  {3036963600U, 7200}, // transition 2519
  {3055712400U, 0}, // transition 2520
  {3068413200U, 7200}, // transition 2521
  {3087162000U, 0}, // transition 2522
  {3099862800U, 7200}, // transition 2523
  {3118611600U, 0}, // transition 2524
  {3131917200U, 7200}, // transition 2525
  {3150061200U, 0}, // transition 2526
  {3163366800U, 7200}, // transition 2527
  {3181510800U, 0}, // transition 2528
  {3194816400U, 7200}, // transition 2529
  {3212960400U, 0}, // transition 2530
  {3226266000U, 7200}, // transition 2531
  {3245014800U, 0}, // transition 2532
  {3257715600U, 7200}, // transition 2533
  {3276464400U, 0}, // transition 2534
  {3289165200U, 7200}, // transition 2535
  {3307914000U, 0}, // transition 2536
  {3321219600U, 7200}, // transition 2537
  {3339363600U, 0}, // transition 2538
  {3352669200U, 7200}, // transition 2539
  {3370813200U, 0}, // transition 2540
  {3384118800U, 7200}, // transition 2541
  {3402867600U, 0}, // transition 2542
  {3415568400U, 7200}, // transition 2543
  {3434317200U, 0}, // transition 2544
  {3447018000U, 7200}, // transition 2545
  {3465766800U, 0}, // transition 2546
  {3479072400U, 7200}, // transition 2547
  {3497216400U, 0}, // transition 2548
  {3510522000U, 7200}, // transition 2549
  {3528666000U, 0}, // transition 2550
  {3541971600U, 7200}, // transition 2551
  {3560115600U, 0}, // transition 2552
  {3573421200U, 7200}, // transition 2553
  {3592170000U, 0}, // transition 2554
  {3604870800U, 7200}, // transition 2555
  {3623619600U, 0}, // transition 2556
  {3636320400U, 7200}, // transition 2557
  {3655069200U, 0}, // transition 2558
  {3668374800U, 7200}, // transition 2559
  {3686518800U, 0}, // transition 2560
  {3699824400U, 7200}, // transition 2561
  {3717968400U, 0}, // transition 2562
  {3731274000U, 7200}, // transition 2563
  {3750022800U, 0}, // transition 2564
  {3762723600U, 7200}, // transition 2565
  {3781472400U, 0}, // transition 2566
  {3794173200U, 7200}, // transition 2567
  {3812922000U, 0}, // transition 2568
  {3825622800U, 7200}, // transition 2569
  {3844371600U, 0}, // transition 2570
  {3857677200U, 7200}, // transition 2571
  {3875821200U, 0}, // transition 2572
  {3889126800U, 7200}, // transition 2573
  {3907270800U, 0}, // transition 2574
  {3920576400U, 7200}, // transition 2575
  {3939325200U, 0}, // transition 2576
  {3952026000U, 7200}, // transition 2577
  {3970774800U, 0}, // transition 2578
  {3983475600U, 7200}, // transition 2579
  {4002224400U, 0}, // transition 2580
  {4015530000U, 7200}, // transition 2581
  {4033674000U, 0}, // transition 2582
  {4046979600U, 7200}, // transition 2583
  {4065123600U, 0}, // transition 2584
  {4078429200U, 7200}, // transition 2585
  {4096573200U, 0}, // transition 2586
  {4109878800U, 7200}, // transition 2587
  {4128627600U, 0}, // transition 2588
  {1767225600U, 43200}, // transition 2589
  {1767225600U, 14400}, // transition 2590
  {1767225600U, 7200}, // transition 2591
  {1774735200U, 10800}, // transition 2592
  {1792875600U, 7200}, // transition 2593
  {1806184800U, 10800}, // transition 2594
  {1824930000U, 7200}, // transition 2595
  {1837634400U, 10800}, // transition 2596
  {1856379600U, 7200}, // transition 2597
  {1869084000U, 10800}, // transition 2598
  {1887829200U, 7200}, // transition 2599
  {1901138400U, 10800}, // transition 2600
  {1919278800U, 7200}, // transition 2601
  {1932588000U, 10800}, // transition 2602
  {1950728400U, 7200}, // transition 2603
  {1964037600U, 10800}, // transition 2604
  {1982782800U, 7200}, // transition 2605
  {1995487200U, 10800}, // transition 2606
  {2014232400U, 7200}, // transition 2607
  {2026936800U, 10800}, // transition 2608
  {2045682000U, 7200}, // transition 2609
  {2058386400U, 10800}, // transition 2610
  {2077131600U, 7200}, // transition 2611
  {2090440800U, 10800}, // transition 2612
  {2108581200U, 7200}, // transition 2613
  {2121890400U, 10800}, // transition 2614
  {2140030800U, 7200}, // transition 2615
  {2153340000U, 10800}, // transition 2616
  {2172085200U, 7200}, // transition 2617
  {2184789600U, 10800}, // transition 2618
  {2203534800U, 7200}, // transition 2619
  {2216239200U, 10800}, // transition 2620
  {2234984400U, 7200}, // transition 2621
  {2248293600U, 10800}, // transition 2622
  {2266434000U, 7200}, // transition 2623
  {2279743200U, 10800}, // transition 2624
  {2297883600U, 7200}, // transition 2625
  {2311192800U, 10800}, // transition 2626
  {2329333200U, 7200}, // transition 2627
  {2342642400U, 10800}, // transition 2628
  {2361387600U, 7200}, // transition 2629
  {2374092000U, 10800}, // transition 2630
  {2392837200U, 7200}, // transition 2631
  {2405541600U, 10800}, // transition 2632
  {2424286800U, 7200}, // transition 2633
  {2437596000U, 10800}, // transition 2634
  {2455736400U, 7200}, // transition 2635
  {2469045600U, 10800}, // transition 2636
  {2487186000U, 7200}, // transition 2637
  {2500495200U, 10800}, // transition 2638
  {2519240400U, 7200}, // transition 2639
  {2531944800U, 10800}, // transition 2640
  {2550690000U, 7200}, // transition 2641
  {2563394400U, 10800}, // transition 2642
  {2582139600U, 7200}, // transition 2643
  {2595448800U, 10800}, // transition 2644
  {2613589200U, 7200}, // transition 2645
  {2626898400U, 10800}, // transition 2646
  {2645038800U, 7200}, // transition 2647
  {2658348000U, 10800}, // transition 2648
  {2676488400U, 7200}, // transition 2649
  {2689797600U, 10800}, // transition 2650
  {2708542800U, 7200}, // transition 2651
  {2721247200U, 10800}, // transition 2652
  {2739992400U, 7200}, // transition 2653
  {2752696800U, 10800}, // transition 2654
  {2771442000U, 7200}, // transition 2655
  {2784751200U, 10800}, // transition 2656
  {2802891600U, 7200}, // transition 2657
  {2816200800U, 10800}, // transition 2658
  {2834341200U, 7200}, // transition 2659
  {2847650400U, 10800}, // transition 2660
  {2866395600U, 7200}, // transition 2661
  {2879100000U, 10800}, // transition 2662
  {2897845200U, 7200}, // transition 2663
  {2910549600U, 10800}, // transition 2664
  {2929294800U, 7200}, // transition 2665
  {2941999200U, 10800}, // transition 2666
  {2960744400U, 7200}, // transition 2667
  {2974053600U, 10800}, // transition 2668
  {2992194000U, 7200}, // transition 2669
  {3005503200U, 10800}, // transition 2670
  {3023643600U, 7200}, // transition 2671
  {3036952800U, 10800}, // transition 2672
  {3055698000U, 7200}, // transition 2673
  {3068402400U, 10800}, // transition 2674
  {3087147600U, 7200}, // transition 2675
  {3099852000U, 10800}, // transition 2676
  {3118597200U, 7200}, // transition 2677
  {3131906400U, 10800}, // transition 2678
  {3150046800U, 7200}, // transition 2679
  {3163356000U, 10800}, // transition 2680
  {3181496400U, 7200}, // transition 2681
  {3194805600U, 10800}, // transition 2682
  {3212946000U, 7200}, // transition 2683
  {3226255200U, 10800}, // transition 2684
  {3245000400U, 7200}, // transition 2685
  {3257704800U, 10800}, // transition 2686
  {3276450000U, 7200}, // transition 2687
  {3289154400U, 10800}, // transition 2688
  {3307899600U, 7200}, // transition 2689
  {3321208800U, 10800}, // transition 2690
  {3339349200U, 7200}, // transition 2691
  {3352658400U, 10800}, // transition 2692
  {3370798800U, 7200}, // transition 2693
  {3384108000U, 10800}, // transition 2694
  {3402853200U, 7200}, // transition 2695
  {3415557600U, 10800}, // transition 2696
  {3434302800U, 7200}, // transition 2697
  {3447007200U, 10800}, // transition 2698
  {3465752400U, 7200}, // transition 2699
  {3479061600U, 10800}, // transition 2700
  {3497202000U, 7200}, // transition 2701
  {3510511200U, 10800}, // transition 2702
  {3528651600U, 7200}, // transition 2703
  {3541960800U, 10800}, // transition 2704
  {3560101200U, 7200}, // transition 2705
  {3573410400U, 10800}, // transition 2706
  {3592155600U, 7200}, // transition 2707
  {3604860000U, 10800}, // transition 2708
  {3623605200U, 7200}, // transition 2709
  {3636309600U, 10800}, // transition 2710
  {3655054800U, 7200}, // transition 2711
  {3668364000U, 10800}, // transition 2712
  {3686504400U, 7200}, // transition 2713
  {3699813600U, 10800}, // transition 2714
  {3717954000U, 7200}, // transition 2715
  {3731263200U, 10800}, // transition 2716
  {3750008400U, 7200}, // transition 2717
  {3762712800U, 10800}, // transition 2718
  {3781458000U, 7200}, // transition 2719
  {3794162400U, 10800}, // transition 2720
  {3812907600U, 7200}, // transition 2721
  {3825612000U, 10800}, // transition 2722
  {3844357200U, 7200}, // transition 2723
  {3857666400U, 10800}, // transition 2724
  {3875806800U, 7200}, // transition 2725
  {3889116000U, 10800}, // transition 2726
  {3907256400U, 7200}, // transition 2727
  {3920565600U, 10800}, // transition 2728
  {3939310800U, 7200}, // transition 2729
  {3952015200U, 10800}, // transition 2730
  {3970760400U, 7200}, // transition 2731
  {3983464800U, 10800}, // transition 2732
  {4002210000U, 7200}, // transition 2733
  {4015519200U, 10800}, // transition 2734
  {4033659600U, 7200}, // transition 2735
  {4046968800U, 10800}, // transition 2736
  {4065109200U, 7200}, // transition 2737
  {4078418400U, 10800}, // transition 2738
  {4096558800U, 7200}, // transition 2739
  {4109868000U, 10800}, // transition 2740
  {4128613200U, 7200}, // transition 2741
  {1767225600U, 21600}, // transition 2742
  {1767225600U, 19800}, // transition 2743
  {1767225600U, 32400}, // transition 2744
  {1767225600U, 7200}, // transition 2745
  {1774746000U, 10800}, // transition 2746
  {1792890000U, 7200}, // transition 2747
  {1806195600U, 10800}, // transition 2748
  {1824944400U, 7200}, // transition 2749
  {1837645200U, 10800}, // transition 2750
  {1856394000U, 7200}, // transition 2751
  {1869094800U, 10800}, // transition 2752
  {1887843600U, 7200}, // transition 2753
  {1901149200U, 10800}, // transition 2754
  {1919293200U, 7200}, // transition 2755
  {1932598800U, 10800}, // transition 2756
  {1950742800U, 7200}, // transition 2757
  {1964048400U, 10800}, // transition 2758
  {1982797200U, 7200}, // transition 2759
  {1995498000U, 10800}, // transition 2760
  {2014246800U, 7200}, // transition 2761
  {2026947600U, 10800}, // transition 2762
  {2045696400U, 7200}, // transition 2763
  {2058397200U, 10800}, // transition 2764
  {2077146000U, 7200}, // transition 2765
  {2090451600U, 10800}, // transition 2766
  {2108595600U, 7200}, // transition 2767
  {2121901200U, 10800}, // transition 2768
  {2140045200U, 7200}, // transition 2769
  {2153350800U, 10800}, // transition 2770
  {2172099600U, 7200}, // transition 2771
  {2184800400U, 10800}, // transition 2772
  {2203549200U, 7200}, // transition 2773
  {2216250000U, 10800}, // transition 2774
  {2234998800U, 7200}, // transition 2775
  {2248304400U, 10800}, // transition 2776
  {2266448400U, 7200}, // transition 2777
  {2279754000U, 10800}, // transition 2778
  {2297898000U, 7200}, // transition 2779
  {2311203600U, 10800}, // transition 2780
  {2329347600U, 7200}, // transition 2781
  {2342653200U, 10800}, // transition 2782
  {2361402000U, 7200}, // transition 2783
  {2374102800U, 10800}, // transition 2784
  {2392851600U, 7200}, // transition 2785
  {2405552400U, 10800}, // transition 2786
  {2424301200U, 7200}, // transition 2787
  {2437606800U, 10800}, // transition 2788
  {2455750800U, 7200}, // transition 2789
  {2469056400U, 10800}, // transition 2790
  {2487200400U, 7200}, // transition 2791
  {2500506000U, 10800}, // transition 2792
  {2519254800U, 7200}, // transition 2793
  {2531955600U, 10800}, // transition 2794
  {2550704400U, 7200}, // transition 2795
  {2563405200U, 10800}, // transition 2796
  {2582154000U, 7200}, // transition 2797
  {2595459600U, 10800}, // transition 2798
  {2613603600U, 7200}, // transition 2799
  {2626909200U, 10800}, // transition 2800
  {2645053200U, 7200}, // transition 2801
  {2658358800U, 10800}, // transition 2802
  {2676502800U, 7200}, // transition 2803
  {2689808400U, 10800}, // transition 2804
  {2708557200U, 7200}, // transition 2805
  {2721258000U, 10800}, // transition 2806
  {2740006800U, 7200}, // transition 2807
  {2752707600U, 10800}, // transition 2808
  {2771456400U, 7200}, // transition 2809
  {2784762000U, 10800}, // transition 2810
  {2802906000U, 7200}, // transition 2811
  {2816211600U, 10800}, // transition 2812
  {2834355600U, 7200}, // transition 2813
  {2847661200U, 10800}, // transition 2814
  {2866410000U, 7200}, // transition 2815
  {2879110800U, 10800}, // transition 2816
  {2897859600U, 7200}, // transition 2817
  {2910560400U, 10800}, // transition 2818
  {2929309200U, 7200}, // transition 2819
  {2942010000U, 10800}, // transition 2820
  {2960758800U, 7200}, // transition 2821
  {2974064400U, 10800}, // transition 2822
  {2992208400U, 7200}, // transition 2823
  {3005514000U, 10800}, // transition 2824
  {3023658000U, 7200}, // transition 2825
  {3036963600U, 10800}, // transition 2826
  {3055712400U, 7200}, // transition 2827
  {3068413200U, 10800}, // transition 2828
  {3087162000U, 7200}, // transition 2829
  {3099862800U, 10800}, // transition 2830
  {3118611600U, 7200}, // transition 2831
  {3131917200U, 10800}, // transition 2832
  {3150061200U, 7200}, // transition 2833
  {3163366800U, 10800}, // transition 2834
  {3181510800U, 7200}, // transition 2835
  {3194816400U, 10800}, // transition 2836
  {3212960400U, 7200}, // transition 2837
  {3226266000U, 10800}, // transition 2838
  {3245014800U, 7200}, // transition 2839
  {3257715600U, 10800}, // transition 2840
  {3276464400U, 7200}, // transition 2841
  {3289165200U, 10800}, // transition 2842
  {3307914000U, 7200}, // transition 2843
  {3321219600U, 10800}, // transition 2844
  {3339363600U, 7200}, // transition 2845
  {3352669200U, 10800}, // transition 2846
  {3370813200U, 7200}, // transition 2847
  {3384118800U, 10800}, // transition 2848
  {3402867600U, 7200}, // transition 2849
  {3415568400U, 10800}, // transition 2850
  {3434317200U, 7200}, // transition 2851
  {3447018000U, 10800}, // transition 2852
  {3465766800U, 7200}, // transition 2853
  {3479072400U, 10800}, // transition 2854
  {3497216400U, 7200}, // transition 2855
  {3510522000U, 10800}, // transition 2856
  {3528666000U, 7200}, // transition 2857
  {3541971600U, 10800}, // transition 2858
  {3560115600U, 7200}, // transition 2859
  {3573421200U, 10800}, // transition 2860
  {3592170000U, 7200}, // transition 2861
  {3604870800U, 10800}, // transition 2862
  {3623619600U, 7200}, // transition 2863
  {3636320400U, 10800}, // transition 2864
  {3655069200U, 7200}, // transition 2865
  {3668374800U, 10800}, // transition 2866
  {3686518800U, 7200}, // transition 2867
  {3699824400U, 10800}, // transition 2868
  {3717968400U, 7200}, // transition 2869
  {3731274000U, 10800}, // transition 2870
  {3750022800U, 7200}, // transition 2871
  {3762723600U, 10800}, // transition 2872
  {3781472400U, 7200}, // transition 2873
  {3794173200U, 10800}, // transition 2874
  {3812922000U, 7200}, // transition 2875
  {3825622800U, 10800}, // transition 2876
  {3844371600U, 7200}, // transition 2877
  {3857677200U, 10800}, // transition 2878
  {3875821200U, 7200}, // transition 2879
  {3889126800U, 10800}, // transition 2880
  {3907270800U, 7200}, // transition 2881
  {3920576400U, 10800}, // transition 2882
  {3939325200U, 7200}, // transition 2883
  {3952026000U, 10800}, // transition 2884
  {3970774800U, 7200}, // transition 2885
  {3983475600U, 10800}, // transition 2886
  {4002224400U, 7200}, // transition 2887
  {4015530000U, 10800}, // transition 2888
  {4033674000U, 7200}, // transition 2889
  {4046979600U, 10800}, // transition 2890
  {4065123600U, 7200}, // transition 2891
  {4078429200U, 10800}, // transition 2892
  {4096573200U, 7200}, // transition 2893
  {4109878800U, 10800}, // transition 2894
  {4128627600U, 7200}, // transition 2895
  {1767225600U, 7200}, // transition 2896
  {1774656000U, 10800}, // transition 2897
  {1792796400U, 7200}, // transition 2898
  {1806105600U, 10800}, // transition 2899
  {1824850800U, 7200}, // transition 2900
  {1837555200U, 10800}, // transition 2901
  {1856300400U, 7200}, // transition 2902
  {1869004800U, 10800}, // transition 2903
  {1887750000U, 7200}, // transition 2904
  {1901059200U, 10800}, // transition 2905
  {1919199600U, 7200}, // transition 2906
  {1932508800U, 10800}, // transition 2907
  {1950649200U, 7200}, // transition 2908
  {1963958400U, 10800}, // transition 2909
  {1982703600U, 7200}, // transition 2910
  {1995408000U, 10800}, // transition 2911
  {2014153200U, 7200}, // transition 2912
  {2026857600U, 10800}, // transition 2913
  {2045602800U, 7200}, // transition 2914
  {2058307200U, 10800}, // transition 2915
  {2077052400U, 7200}, // transition 2916
  {2090361600U, 10800}, // transition 2917
  {2107897200U, 7200}, // transition 2918
  {2121811200U, 10800}, // transition 2919
  {2138742000U, 7200}, // transition 2920
  {2153260800U, 10800}, // transition 2921
  {2168982000U, 7200}, // transition 2922
  {2184710400U, 10800}, // transition 2923
  {2199826800U, 7200}, // transition 2924
  {2216160000U, 10800}, // transition 2925
  {2230066800U, 7200}, // transition 2926
  {2234304000U, 10800}, // transition 2927
  {2234905200U, 7200}, // transition 2928
  {2248214400U, 10800}, // transition 2929
  {2260911600U, 7200}, // transition 2930
  {2264544000U, 10800}, // transition 2931
  {2266354800U, 7200}, // transition 2932
  {2279664000U, 10800}, // transition 2933
  {2291756400U, 7200}, // transition 2934
  {2295388800U, 10800}, // transition 2935
  {2297804400U, 7200}, // transition 2936
  {2311113600U, 10800}, // transition 2937
  {2321996400U, 7200}, // transition 2938
  {2326233600U, 10800}, // transition 2939
  {2329254000U, 7200}, // transition 2940
  {2342563200U, 10800}, // transition 2941
  {2352841200U, 7200}, // transition 2942
  {2356473600U, 10800}, // transition 2943
  {2361308400U, 7200}, // transition 2944
  {2374012800U, 10800}, // transition 2945
  {2383686000U, 7200}, // transition 2946
  {2387318400U, 10800}, // transition 2947
  {2392758000U, 7200}, // transition 2948
  {2405462400U, 10800}, // transition 2949
  {2413926000U, 7200}, // transition 2950
  {2418163200U, 10800}, // transition 2951
  {2424207600U, 7200}, // transition 2952
  {2437516800U, 10800}, // transition 2953
  {2444770800U, 7200}, // transition 2954
  {2448403200U, 10800}, // transition 2955
  {2455657200U, 7200}, // transition 2956
  {2468966400U, 10800}, // transition 2957
  {2475010800U, 7200}, // transition 2958
  {2479248000U, 10800}, // transition 2959
  {2487106800U, 7200}, // transition 2960
  {2500416000U, 10800}, // transition 2961
  {2505855600U, 7200}, // transition 2962
  {2509488000U, 10800}, // transition 2963
  {2519161200U, 7200}, // transition 2964
  {2531865600U, 10800}, // transition 2965
  {2536700400U, 7200}, // transition 2966
  {2540332800U, 10800}, // transition 2967
  {2550610800U, 7200}, // transition 2968
  {2563315200U, 10800}, // transition 2969
  {2566940400U, 7200}, // transition 2970
  {2571177600U, 10800}, // transition 2971
  {2582060400U, 7200}, // transition 2972
  {2595369600U, 10800}, // transition 2973
  {2597785200U, 7200}, // transition 2974
  {2601417600U, 10800}, // transition 2975
  {2613510000U, 7200}, // transition 2976
  {2626819200U, 10800}, // transition 2977
  {2628025200U, 7200}, // transition 2978
  {2632262400U, 10800}, // transition 2979
  {2644959600U, 7200}, // transition 2980
  {2658268800U, 10800}, // transition 2981
  {2658870000U, 7200}, // transition 2982
  {2663107200U, 10800}, // transition 2983
  {2676409200U, 7200}, // transition 2984
  {2693347200U, 10800}, // transition 2985
  {2708463600U, 7200}, // transition 2986
  {2724192000U, 10800}, // transition 2987
  {2739913200U, 7200}, // transition 2988
  {2754432000U, 10800}, // transition 2989
  {2771362800U, 7200}, // transition 2990
  {2785276800U, 10800}, // transition 2991
  {2802812400U, 7200}, // transition 2992
  {2816121600U, 10800}, // transition 2993
  {2834262000U, 7200}, // transition 2994
  {2847571200U, 10800}, // transition 2995
  {2866316400U, 7200}, // transition 2996
  {2879020800U, 10800}, // transition 2997
  {2897766000U, 7200}, // transition 2998
  {2910470400U, 10800}, // transition 2999
  {2929215600U, 7200}, // transition 3000
  {2941920000U, 10800}, // transition 3001
  {2960665200U, 7200}, // transition 3002
  {2973974400U, 10800}, // transition 3003
  {2992114800U, 7200}, // transition 3004
  {3005424000U, 10800}, // transition 3005
  {3023564400U, 7200}, // transition 3006
  {3036873600U, 10800}, // transition 3007
  {3055618800U, 7200}, // transition 3008
  {3068323200U, 10800}, // transition 3009
  {3087068400U, 7200}, // transition 3010
  {3099772800U, 10800}, // transition 3011
  {3117913200U, 7200}, // transition 3012
  {3131827200U, 10800}, // transition 3013
  {3148758000U, 7200}, // transition 3014
  {3163276800U, 10800}, // transition 3015
  {3179602800U, 7200}, // transition 3016
  {3194726400U, 10800}, // transition 3017
  {3209842800U, 7200}, // transition 3018
  {3226176000U, 10800}, // transition 3019
  {3240687600U, 7200}, // transition 3020
  {3244320000U, 10800}, // transition 3021
  {3244921200U, 7200}, // transition 3022
  {3257625600U, 10800}, // transition 3023
  {3271532400U, 7200}, // transition 3024
  {3275164800U, 10800}, // transition 3025
  {3276370800U, 7200}, // transition 3026
  {3289075200U, 10800}, // transition 3027
  {3301772400U, 7200}, // transition 3028
  {3306009600U, 10800}, // transition 3029
  {3307820400U, 7200}, // transition 3030
  {3321129600U, 10800}, // transition 3031
  {3332617200U, 7200}, // transition 3032
  {3336249600U, 10800}, // transition 3033
  {3339270000U, 7200}, // transition 3034
  {3352579200U, 10800}, // transition 3035
  {3362857200U, 7200}, // transition 3036
  {3367094400U, 10800}, // transition 3037
  {3370719600U, 7200}, // transition 3038
  {3384028800U, 10800}, // transition 3039
  {3393702000U, 7200}, // transition 3040
  {3397939200U, 10800}, // transition 3041
  {3402774000U, 7200}, // transition 3042
  {3415478400U, 10800}, // transition 3043
  {3424546800U, 7200}, // transition 3044
  {3428179200U, 10800}, // transition 3045
  {3434223600U, 7200}, // transition 3046
  {3446928000U, 10800}, // transition 3047
  {3454786800U, 7200}, // transition 3048
  {3459024000U, 10800}, // transition 3049
  {3465673200U, 7200}, // transition 3050
  {3478982400U, 10800}, // transition 3051
  {3485631600U, 7200}, // transition 3052
  {3489264000U, 10800}, // transition 3053
  {3497122800U, 7200}, // transition 3054
  {3510432000U, 10800}, // transition 3055
  {3516476400U, 7200}, // transition 3056
  {3520108800U, 10800}, // transition 3057
  {3528572400U, 7200}, // transition 3058
  {3541881600U, 10800}, // transition 3059
  {3546716400U, 7200}, // transition 3060
  {3550953600U, 10800}, // transition 3061
  {3560022000U, 7200}, // transition 3062
  {3573331200U, 10800}, // transition 3063
  {3577561200U, 7200}, // transition 3064
  {3581193600U, 10800}, // transition 3065
  {3592076400U, 7200}, // transition 3066
  {3604780800U, 10800}, // transition 3067
  {3607801200U, 7200}, // transition 3068
  {3612038400U, 10800}, // transition 3069
  {3623526000U, 7200}, // transition 3070
  {3636230400U, 10800}, // transition 3071
  {3638646000U, 7200}, // transition 3072
  {3642883200U, 10800}, // transition 3073
  {3654975600U, 7200}, // transition 3074
  {3668284800U, 10800}, // transition 3075
  {3669490800U, 7200}, // transition 3076
  {3673123200U, 10800}, // transition 3077
  {3686425200U, 7200}, // transition 3078
  {3699734400U, 10800}, // transition 3079
  {3717874800U, 7200}, // transition 3080
  {3731184000U, 10800}, // transition 3081
  {3749929200U, 7200}, // transition 3082
  {3762633600U, 10800}, // transition 3083
  {3781378800U, 7200}, // transition 3084
  {3794083200U, 10800}, // transition 3085
  {3812828400U, 7200}, // transition 3086
  {3825532800U, 10800}, // transition 3087
  {3844278000U, 7200}, // transition 3088
  {3857587200U, 10800}, // transition 3089
  {3875727600U, 7200}, // transition 3090
  {3889036800U, 10800}, // transition 3091
  {3907177200U, 7200}, // transition 3092
  {3920486400U, 10800}, // transition 3093
  {3939231600U, 7200}, // transition 3094
  {3951936000U, 10800}, // transition 3095
  {3970681200U, 7200}, // transition 3096
  {3983385600U, 10800}, // transition 3097
  {4002130800U, 7200}, // transition 3098
  {4015440000U, 10800}, // transition 3099
  {4033580400U, 7200}, // transition 3100
  {4046889600U, 10800}, // transition 3101
  {4065030000U, 7200}, // transition 3102
  {4078339200U, 10800}, // transition 3103
  {4096479600U, 7200}, // transition 3104
  {4109788800U, 10800}, // transition 3105
  {4128534000U, 7200}, // transition 3106
  {1767225600U, 7200}, // transition 3107
  {1774569600U, 10800}, // transition 3108
  {1792882800U, 7200}, // transition 3109
  {1806019200U, 10800}, // transition 3110
  {1824937200U, 7200}, // transition 3111
  {1837468800U, 10800}, // transition 3112
  {1856386800U, 7200}, // transition 3113
  {1868918400U, 10800}, // transition 3114
  {1887836400U, 7200}, // transition 3115
  {1900972800U, 10800}, // transition 3116
  {1919286000U, 7200}, // transition 3117
  {1932422400U, 10800}, // transition 3118
  {1950735600U, 7200}, // transition 3119
  {1963872000U, 10800}, // transition 3120
  {1982790000U, 7200}, // transition 3121
  {1995321600U, 10800}, // transition 3122
  {2014239600U, 7200}, // transition 3123
  {2026771200U, 10800}, // transition 3124
  {2045689200U, 7200}, // transition 3125
  {2058220800U, 10800}, // transition 3126
  {2077138800U, 7200}, // transition 3127
  {2090275200U, 10800}, // transition 3128
  {2108588400U, 7200}, // transition 3129
  {2121724800U, 10800}, // transition 3130
  {2140038000U, 7200}, // transition 3131
  {2153174400U, 10800}, // transition 3132
  {2172092400U, 7200}, // transition 3133
  {2184624000U, 10800}, // transition 3134
  {2203542000U, 7200}, // transition 3135
  {2216073600U, 10800}, // transition 3136
  {2234991600U, 7200}, // transition 3137
  {2248128000U, 10800}, // transition 3138
  {2266441200U, 7200}, // transition 3139
  {2279577600U, 10800}, // transition 3140
  {2297890800U, 7200}, // transition 3141
  {2311027200U, 10800}, // transition 3142
  {2329340400U, 7200}, // transition 3143
  {2342476800U, 10800}, // transition 3144
  {2361394800U, 7200}, // transition 3145
  {2373926400U, 10800}, // transition 3146
  {2392844400U, 7200}, // transition 3147
  {2405376000U, 10800}, // transition 3148
  {2424294000U, 7200}, // transition 3149
  {2437430400U, 10800}, // transition 3150
  {2455743600U, 7200}, // transition 3151
  {2468880000U, 10800}, // transition 3152
  {2487193200U, 7200}, // transition 3153
  {2500329600U, 10800}, // transition 3154
  {2519247600U, 7200}, // transition 3155
  {2531779200U, 10800}, // transition 3156
  {2550697200U, 7200}, // transition 3157
  {2563228800U, 10800}, // transition 3158
  {2582146800U, 7200}, // transition 3159
  {2595283200U, 10800}, // transition 3160
  {2613596400U, 7200}, // transition 3161
  {2626732800U, 10800}, // transition 3162
  {2645046000U, 7200}, // transition 3163
  {2658182400U, 10800}, // transition 3164
  {2676495600U, 7200}, // transition 3165
  {2689632000U, 10800}, // transition 3166
  {2708550000U, 7200}, // transition 3167
  {2721081600U, 10800}, // transition 3168
  {2739999600U, 7200}, // transition 3169
  {2752531200U, 10800}, // transition 3170
  {2771449200U, 7200}, // transition 3171
  {2784585600U, 10800}, // transition 3172
  {2802898800U, 7200}, // transition 3173
  {2816035200U, 10800}, // transition 3174
  {2834348400U, 7200}, // transition 3175
  {2847484800U, 10800}, // transition 3176
  {2866402800U, 7200}, // transition 3177
  {2878934400U, 10800}, // transition 3178
  {2897852400U, 7200}, // transition 3179
  {2910384000U, 10800}, // transition 3180
  {2929302000U, 7200}, // transition 3181
  {2941833600U, 10800}, // transition 3182
  {2960751600U, 7200}, // transition 3183
  {2973888000U, 10800}, // transition 3184
  {2992201200U, 7200}, // transition 3185
  {3005337600U, 10800}, // transition 3186
  {3023650800U, 7200}, // transition 3187
  {3036787200U, 10800}, // transition 3188
  {3055705200U, 7200}, // transition 3189
  {3068236800U, 10800}, // transition 3190
  {3087154800U, 7200}, // transition 3191
  {3099686400U, 10800}, // transition 3192
  {3118604400U, 7200}, // transition 3193
  {3131740800U, 10800}, // transition 3194
  {3150054000U, 7200}, // transition 3195
  {3163190400U, 10800}, // transition 3196
  {3181503600U, 7200}, // transition 3197
  {3194640000U, 10800}, // transition 3198
  {3212953200U, 7200}, // transition 3199
  {3226089600U, 10800}, // transition 3200
  {3245007600U, 7200}, // transition 3201
  {3257539200U, 10800}, // transition 3202
  {3276457200U, 7200}, // transition 3203
  {3288988800U, 10800}, // transition 3204
  {3307906800U, 7200}, // transition 3205
  {3321043200U, 10800}, // transition 3206
  {3339356400U, 7200}, // transition 3207
  {3352492800U, 10800}, // transition 3208
  {3370806000U, 7200}, // transition 3209
  {3383942400U, 10800}, // transition 3210
  {3402860400U, 7200}, // transition 3211
  {3415392000U, 10800}, // transition 3212
  {3434310000U, 7200}, // transition 3213
  {3446841600U, 10800}, // transition 3214
  {3465759600U, 7200}, // transition 3215
  {3478896000U, 10800}, // transition 3216
  {3497209200U, 7200}, // transition 3217
  {3510345600U, 10800}, // transition 3218
  {3528658800U, 7200}, // transition 3219
  {3541795200U, 10800}, // transition 3220
  {3560108400U, 7200}, // transition 3221
  {3573244800U, 10800}, // transition 3222
  {3592162800U, 7200}, // transition 3223
  {3604694400U, 10800}, // transition 3224
  {3623612400U, 7200}, // transition 3225
  {3636144000U, 10800}, // transition 3226
  {3655062000U, 7200}, // transition 3227
  {3668198400U, 10800}, // transition 3228
  {3686511600U, 7200}, // transition 3229
  {3699648000U, 10800}, // transition 3230
  {3717961200U, 7200}, // transition 3231
  {3731097600U, 10800}, // transition 3232
  {3750015600U, 7200}, // transition 3233
  {3762547200U, 10800}, // transition 3234
  {3781465200U, 7200}, // transition 3235
  {3793996800U, 10800}, // transition 3236
  {3812914800U, 7200}, // transition 3237
  {3825446400U, 10800}, // transition 3238
  {3844364400U, 7200}, // transition 3239
  {3857500800U, 10800}, // transition 3240
  {3875814000U, 7200}, // transition 3241
  {3888950400U, 10800}, // transition 3242
  {3907263600U, 7200}, // transition 3243
  {3920400000U, 10800}, // transition 3244
  {3939318000U, 7200}, // transition 3245
  {3951849600U, 10800}, // transition 3246
  {3970767600U, 7200}, // transition 3247
  {3983299200U, 10800}, // transition 3248
  {4002217200U, 7200}, // transition 3249
  {4015353600U, 10800}, // transition 3250
  {4033666800U, 7200}, // transition 3251
  {4046803200U, 10800}, // transition 3252
  {4065116400U, 7200}, // transition 3253
  {4078252800U, 10800}, // transition 3254
  {4096566000U, 7200}, // transition 3255
  {4109702400U, 10800}, // transition 3256
  {4128620400U, 7200}, // transition 3257
  {1767225600U, 16200}, // transition 3258
  {1767225600U, 20700}, // transition 3259
  {1767225600U, 39600}, // transition 3260
  {1767225600U, 23400}, // transition 3261
  {1767225600U, 12600}, // transition 3262
  {1767225600U, -3600}, // transition 3263
  {1774746000U, 0}, // transition 3264
  {1792890000U, -3600}, // transition 3265
  {1806195600U, 0}, // transition 3266
  {1824944400U, -3600}, // transition 3267
  {1837645200U, 0}, // transition 3268
  {1856394000U, -3600}, // transition 3269
  {1869094800U, 0}, // transition 3270
  {1887843600U, -3600}, // transition 3271
  {1901149200U, 0}, // transition 3272
  {1919293200U, -3600}, // transition 3273
  {1932598800U, 0}, // transition 3274
  {1950742800U, -3600}, // transition 3275
  {1964048400U, 0}, // transition 3276
  {1982797200U, -3600}, // transition 3277
  {1995498000U, 0}, // transition 3278
  {2014246800U, -3600}, // transition 3279
  {2026947600U, 0}, // transition 3280
  {2045696400U, -3600}, // transition 3281
  {2058397200U, 0}, // transition 3282
  {2077146000U, -3600}, // transition 3283
  {2090451600U, 0}, // transition 3284
  {2108595600U, -3600}, // transition 3285
  {2121901200U, 0}, // transition 3286
  {2140045200U, -3600}, // transition 3287
  {2153350800U, 0}, // transition 3288
  {2172099600U, -3600}, // transition 3289
  {2184800400U, 0}, // transition 3290
  {2203549200U, -3600}, // transition 3291
  {2216250000U, 0}, // transition 3292
  {2234998800U, -3600}, // transition 3293
  {2248304400U, 0}, // transition 3294
  {2266448400U, -3600}, // transition 3295
  {2279754000U, 0}, // transition 3296
  {2297898000U, -3600}, // transition 3297
  {2311203600U, 0}, // transition 3298
  {2329347600U, -3600}, // transition 3299
  {2342653200U, 0}, // transition 3300
  {2361402000U, -3600}, // transition 3301
  {2374102800U, 0}, // transition 3302
  {2392851600U, -3600}, // transition 3303
  {2405552400U, 0}, // transition 3304
  {2424301200U, -3600}, // transition 3305
  {2437606800U, 0}, // transition 3306
  {2455750800U, -3600}, // transition 3307
  {2469056400U, 0}, // transition 3308
  {2487200400U, -3600}, // transition 3309
  {2500506000U, 0}, // transition 3310
  {2519254800U, -3600}, // transition 3311
  {2531955600U, 0}, // transition 3312
  {2550704400U, -3600}, // transition 3313
  {2563405200U, 0}, // transition 3314
  {2582154000U, -3600}, // transition 3315
  {2595459600U, 0}, // transition 3316
  {2613603600U, -3600}, // transition 3317
  {2626909200U, 0}, // transition 3318
  {2645053200U, -3600}, // transition 3319
  {2658358800U, 0}, // transition 3320
  {2676502800U, -3600}, // transition 3321
  {2689808400U, 0}, // transition 3322
  {2708557200U, -3600}, // transition 3323
  {2721258000U, 0}, // transition 3324
  {2740006800U, -3600}, // transition 3325
  {2752707600U, 0}, // transition 3326
  {2771456400U, -3600}, // transition 3327
  {2784762000U, 0}, // transition 3328
  {2802906000U, -3600}, // transition 3329
  {2816211600U, 0}, // transition 3330
  {2834355600U, -3600}, // transition 3331
  {2847661200U, 0}, // transition 3332
  {2866410000U, -3600}, // transition 3333
  {2879110800U, 0}, // transition 3334
  {2897859600U, -3600}, // transition 3335
  {2910560400U, 0}, // transition 3336
  {2929309200U, -3600}, // transition 3337
  {2942010000U, 0}, // transition 3338
  {2960758800U, -3600}, // transition 3339
  {2974064400U, 0}, // transition 3340
  {2992208400U, -3600}, // transition 3341
  {3005514000U, 0}, // transition 3342
  {3023658000U, -3600}, // transition 3343
  {3036963600U, 0}, // transition 3344
  {3055712400U, -3600}, // transition 3345
  {3068413200U, 0}, // transition 3346
  {3087162000U, -3600}, // transition 3347
  {3099862800U, 0}, // transition 3348
  {3118611600U, -3600}, // transition 3349
  {3131917200U, 0}, // transition 3350
  {3150061200U, -3600}, // transition 3351
  {3163366800U, 0}, // transition 3352
  {3181510800U, -3600}, // transition 3353
  {3194816400U, 0}, // transition 3354
  {3212960400U, -3600}, // transition 3355
  {3226266000U, 0}, // transition 3356
  {3245014800U, -3600}, // transition 3357
  {3257715600U, 0}, // transition 3358
  {3276464400U, -3600}, // transition 3359
  {3289165200U, 0}, // transition 3360
  {3307914000U, -3600}, // transition 3361
  {3321219600U, 0}, // transition 3362
  {3339363600U, -3600}, // transition 3363
  {3352669200U, 0}, // transition 3364
  {3370813200U, -3600}, // transition 3365
  {3384118800U, 0}, // transition 3366
  {3402867600U, -3600}, // transition 3367
  {3415568400U, 0}, // transition 3368
  {3434317200U, -3600}, // transition 3369
  {3447018000U, 0}, // transition 3370
  {3465766800U, -3600}, // transition 3371
  {3479072400U, 0}, // transition 3372
  {3497216400U, -3600}, // transition 3373
  {3510522000U, 0}, // transition 3374
  {3528666000U, -3600}, // transition 3375
  {3541971600U, 0}, // transition 3376
  {3560115600U, -3600}, // transition 3377
  {3573421200U, 0}, // transition 3378
  {3592170000U, -3600}, // transition 3379
  {3604870800U, 0}, // transition 3380
  {3623619600U, -3600}, // transition 3381
  {3636320400U, 0}, // transition 3382
  {3655069200U, -3600}, // transition 3383
  {3668374800U, 0}, // transition 3384
  {3686518800U, -3600}, // transition 3385
  {3699824400U, 0}, // transition 3386
  {3717968400U, -3600}, // transition 3387
  {3731274000U, 0}, // transition 3388
  {3750022800U, -3600}, // transition 3389
  {3762723600U, 0}, // transition 3390
  {3781472400U, -3600}, // transition 3391
  {3794173200U, 0}, // transition 3392
  {3812922000U, -3600}, // transition 3393
  {3825622800U, 0}, // transition 3394
  {3844371600U, -3600}, // transition 3395
  {3857677200U, 0}, // transition 3396
  {3875821200U, -3600}, // transition 3397
  {3889126800U, 0}, // transition 3398
  {3907270800U, -3600}, // transition 3399
  {3920576400U, 0}, // transition 3400
  {3939325200U, -3600}, // transition 3401
  {3952026000U, 0}, // transition 3402
  {3970774800U, -3600}, // transition 3403
  {3983475600U, 0}, // transition 3404
  {4002224400U, -3600}, // transition 3405
  {4015530000U, 0}, // transition 3406
  {4033674000U, -3600}, // transition 3407
  {4046979600U, 0}, // transition 3408
  {4065123600U, -3600}, // transition 3409
  {4078429200U, 0}, // transition 3410
  {4096573200U, -3600}, // transition 3411
  {4109878800U, 0}, // transition 3412
  {4128627600U, -3600}, // transition 3413
  {1767225600U, 0}, // transition 3414
  {1774746000U, 3600}, // transition 3415
  {1792890000U, 0}, // transition 3416
  {1806195600U, 3600}, // transition 3417
  {1824944400U, 0}, // transition 3418
  {1837645200U, 3600}, // transition 3419
  {1856394000U, 0}, // transition 3420
  {1869094800U, 3600}, // transition 3421
  {1887843600U, 0}, // transition 3422
  {1901149200U, 3600}, // transition 3423
  {1919293200U, 0}, // transition 3424
  {1932598800U, 3600}, // transition 3425
  {1950742800U, 0}, // transition 3426
  {1964048400U, 3600}, // transition 3427
  {1982797200U, 0}, // transition 3428
  {1995498000U, 3600}, // transition 3429
  {2014246800U, 0}, // transition 3430
  {2026947600U, 3600}, // transition 3431
  {2045696400U, 0}, // transition 3432
  {2058397200U, 3600}, // transition 3433
  {2077146000U, 0}, // transition 3434
  {2090451600U, 3600}, // transition 3435
  {2108595600U, 0}, // transition 3436
  {2121901200U, 3600}, // transition 3437
  {2140045200U, 0}, // transition 3438
  {2153350800U, 3600}, // transition 3439
  {2172099600U, 0}, // transition 3440
  {2184800400U, 3600}, // transition 3441
  {2203549200U, 0}, // transition 3442
  {2216250000U, 3600}, // transition 3443
  {2234998800U, 0}, // transition 3444
  {2248304400U, 3600}, // transition 3445
  {2266448400U, 0}, // transition 3446
  {2279754000U, 3600}, // transition 3447
  {2297898000U, 0}, // transition 3448
  {2311203600U, 3600}, // transition 3449
  {2329347600U, 0}, // transition 3450
  {2342653200U, 3600}, // transition 3451
  {2361402000U, 0}, // transition 3452
  {2374102800U, 3600}, // transition 3453
  {2392851600U, 0}, // transition 3454
  {2405552400U, 3600}, // transition 3455
  {2424301200U, 0}, // transition 3456
  {2437606800U, 3600}, // transition 3457
  {2455750800U, 0}, // transition 3458
  {2469056400U, 3600}, // transition 3459
  {2487200400U, 0}, // transition 3460
  {2500506000U, 3600}, // transition 3461
  {2519254800U, 0}, // transition 3462
  {2531955600U, 3600}, // transition 3463
  {2550704400U, 0}, // transition 3464
  {2563405200U, 3600}, // transition 3465
  {2582154000U, 0}, // transition 3466
  {2595459600U, 3600}, // transition 3467
  {2613603600U, 0}, // transition 3468
  {2626909200U, 3600}, // transition 3469
  {2645053200U, 0}, // transition 3470
  {2658358800U, 3600}, // transition 3471
  {2676502800U, 0}, // transition 3472
  {2689808400U, 3600}, // transition 3473
  {2708557200U, 0}, // transition 3474
  {2721258000U, 3600}, // transition 3475
  {2740006800U, 0}, // transition 3476
  {2752707600U, 3600}, // transition 3477
  {2771456400U, 0}, // transition 3478
  {2784762000U, 3600}, // transition 3479
  {2802906000U, 0}, // transition 3480
  {2816211600U, 3600}, // transition 3481
  {2834355600U, 0}, // transition 3482
  {2847661200U, 3600}, // transition 3483
  {2866410000U, 0}, // transition 3484
  {2879110800U, 3600}, // transition 3485
  {2897859600U, 0}, // transition 3486
  {2910560400U, 3600}, // transition 3487
  {2929309200U, 0}, // transition 3488
  {2942010000U, 3600}, // transition 3489
  {2960758800U, 0}, // transition 3490
  {2974064400U, 3600}, // transition 3491
  {2992208400U, 0}, // transition 3492
  {3005514000U, 3600}, // transition 3493
  {3023658000U, 0}, // transition 3494
  {3036963600U, 3600}, // transition 3495
  {3055712400U, 0}, // transition 3496
  {3068413200U, 3600}, // transition 3497
  {3087162000U, 0}, // transition 3498
  {3099862800U, 3600}, // transition 3499
  {3118611600U, 0}, // transition 3500
  {3131917200U, 3600}, // transition 3501
  {3150061200U, 0}, // transition 3502
  {3163366800U, 3600}, // transition 3503
  {3181510800U, 0}, // transition 3504
  {3194816400U, 3600}, // transition 3505
  {3212960400U, 0}, // transition 3506
  {3226266000U, 3600}, // transition 3507
  {3245014800U, 0}, // transition 3508
  {3257715600U, 3600}, // transition 3509
  {3276464400U, 0}, // transition 3510
  {3289165200U, 3600}, // transition 3511
  {3307914000U, 0}, // transition 3512
  {3321219600U, 3600}, // transition 3513
  {3339363600U, 0}, // transition 3514
  {3352669200U, 3600}, // transition 3515
  {3370813200U, 0}, // transition 3516
  {3384118800U, 3600}, // transition 3517
  {3402867600U, 0}, // transition 3518
  {3415568400U, 3600}, // transition 3519
  {3434317200U, 0}, // transition 3520
  {3447018000U, 3600}, // transition 3521
  {3465766800U, 0}, // transition 3522
  {3479072400U, 3600}, // transition 3523
  {3497216400U, 0}, // transition 3524
  {3510522000U, 3600}, // transition 3525
  {3528666000U, 0}, // transition 3526
  {3541971600U, 3600}, // transition 3527
  {3560115600U, 0}, // transition 3528
  {3573421200U, 3600}, // transition 3529
  {3592170000U, 0}, // transition 3530
  {3604870800U, 3600}, // transition 3531
  {3623619600U, 0}, // transition 3532
  {3636320400U, 3600}, // transition 3533
  {3655069200U, 0}, // transition 3534
  {3668374800U, 3600}, // transition 3535
  {3686518800U, 0}, // transition 3536
  {3699824400U, 3600}, // transition 3537
  {3717968400U, 0}, // transition 3538
  {3731274000U, 3600}, // transition 3539
  {3750022800U, 0}, // transition 3540
  {3762723600U, 3600}, // transition 3541
  {3781472400U, 0}, // transition 3542
  {3794173200U, 3600}, // transition 3543
  {3812922000U, 0}, // transition 3544
  {3825622800U, 3600}, // transition 3545
  {3844371600U, 0}, // transition 3546
  {3857677200U, 3600}, // transition 3547
  {3875821200U, 0}, // transition 3548
  {3889126800U, 3600}, // transition 3549
  {3907270800U, 0}, // transition 3550
  {3920576400U, 3600}, // transition 3551
  {3939325200U, 0}, // transition 3552
  {3952026000U, 3600}, // transition 3553
  {3970774800U, 0}, // transition 3554
  {3983475600U, 3600}, // transition 3555
  {4002224400U, 0}, // transition 3556
  {4015530000U, 3600}, // transition 3557
  {4033674000U, 0}, // transition 3558
  {4046979600U, 3600}, // transition 3559
  {4065123600U, 0}, // transition 3560
  {4078429200U, 3600}, // transition 3561
  {4096573200U, 0}, // transition 3562
  {4109878800U, 3600}, // transition 3563
  {4128627600U, 0}, // transition 3564
  {1767225600U, -3600}, // transition 3565
  {1767225600U, 37800}, // transition 3566
  {1775320200U, 34200}, // transition 3567
  {1791045000U, 37800}, // transition 3568
  {1806769800U, 34200}, // transition 3569
  {1822494600U, 37800}, // transition 3570
  {1838219400U, 34200}, // transition 3571
  {1853944200U, 37800}, // transition 3572
  {1869669000U, 34200}, // transition 3573
  {1885998600U, 37800}, // transition 3574
  {1901723400U, 34200}, // transition 3575
  {1917448200U, 37800}, // transition 3576
  {1933173000U, 34200}, // transition 3577
  {1948897800U, 37800}, // transition 3578
  {1964622600U, 34200}, // transition 3579
  {1980347400U, 37800}, // transition 3580
  {1996072200U, 34200}, // transition 3581
  {2011797000U, 37800}, // transition 3582
  {2027521800U, 34200}, // transition 3583
  {2043246600U, 37800}, // transition 3584
  {2058971400U, 34200}, // transition 3585
  {2075301000U, 37800}, // transition 3586
  {2091025800U, 34200}, // transition 3587
  {2106750600U, 37800}, // transition 3588
  {2122475400U, 34200}, // transition 3589
  {2138200200U, 37800}, // transition 3590
  {2153925000U, 34200}, // transition 3591
  {2169649800U, 37800}, // transition 3592
  {2185374600U, 34200}, // transition 3593
  {2201099400U, 37800}, // transition 3594
  {2216824200U, 34200}, // transition 3595
  {2233153800U, 37800}, // transition 3596
  {2248878600U, 34200}, // transition 3597
  {2264603400U, 37800}, // transition 3598
  {2280328200U, 34200}, // transition 3599
  {2296053000U, 37800}, // transition 3600
  {2311777800U, 34200}, // transition 3601
  {2327502600U, 37800}, // transition 3602
  {2343227400U, 34200}, // transition 3603
  {2358952200U, 37800}, // transition 3604
  {2374677000U, 34200}, // transition 3605
  {2390401800U, 37800}, // transition 3606
  {2406126600U, 34200}, // transition 3607
  {2422456200U, 37800}, // transition 3608
  {2438181000U, 34200}, // transition 3609
  {2453905800U, 37800}, // transition 3610
  {2469630600U, 34200}, // transition 3611
  {2485355400U, 37800}, // transition 3612
  {2501080200U, 34200}, // transition 3613
  {2516805000U, 37800}, // transition 3614
  {2532529800U, 34200}, // transition 3615
  {2548254600U, 37800}, // transition 3616
  {2563979400U, 34200}, // transition 3617
  {2579704200U, 37800}, // transition 3618
  {2596033800U, 34200}, // transition 3619
  {2611758600U, 37800}, // transition 3620
  {2627483400U, 34200}, // transition 3621
  {2643208200U, 37800}, // transition 3622
  {2658933000U, 34200}, // transition 3623
  {2674657800U, 37800}, // transition 3624
  {2690382600U, 34200}, // transition 3625
  {2706107400U, 37800}, // transition 3626
  {2721832200U, 34200}, // transition 3627
  {2737557000U, 37800}, // transition 3628
  {2753281800U, 34200}, // transition 3629
  {2769611400U, 37800}, // transition 3630
  {2785336200U, 34200}, // transition 3631
  {2801061000U, 37800}, // transition 3632
  {2816785800U, 34200}, // transition 3633
  {2832510600U, 37800}, // transition 3634
  {2848235400U, 34200}, // transition 3635
  {2863960200U, 37800}, // transition 3636
  {2879685000U, 34200}, // transition 3637
  {2895409800U, 37800}, // transition 3638
  {2911134600U, 34200}, // transition 3639
  {2926859400U, 37800}, // transition 3640
  {2942584200U, 34200}, // transition 3641
  {2958913800U, 37800}, // transition 3642
  {2974638600U, 34200}, // transition 3643
  {2990363400U, 37800}, // transition 3644
  {3006088200U, 34200}, // transition 3645
  {3021813000U, 37800}, // transition 3646
  {3037537800U, 34200}, // transition 3647
  {3053262600U, 37800}, // transition 3648
  {3068987400U, 34200}, // transition 3649
  {3084712200U, 37800}, // transition 3650
  {3100437000U, 34200}, // transition 3651
  {3116766600U, 37800}, // transition 3652
  {3132491400U, 34200}, // transition 3653
  {3148216200U, 37800}, // transition 3654
  {3163941000U, 34200}, // transition 3655
  {3179665800U, 37800}, // transition 3656
  {3195390600U, 34200}, // transition 3657
  {3211115400U, 37800}, // transition 3658
  {3226840200U, 34200}, // transition 3659
  {3242565000U, 37800}, // transition 3660
  {3258289800U, 34200}, // transition 3661
  {3274014600U, 37800}, // transition 3662
  {3289739400U, 34200}, // transition 3663
  {3306069000U, 37800}, // transition 3664
  {3321793800U, 34200}, // transition 3665
  {3337518600U, 37800}, // transition 3666
  {3353243400U, 34200}, // transition 3667
  {3368968200U, 37800}, // transition 3668
  {3384693000U, 34200}, // transition 3669
  {3400417800U, 37800}, // transition 3670
  {3416142600U, 34200}, // transition 3671
  {3431867400U, 37800}, // transition 3672
  {3447592200U, 34200}, // transition 3673
  {3463317000U, 37800}, // transition 3674
  {3479646600U, 34200}, // transition 3675
  {3495371400U, 37800}, // transition 3676
  {3511096200U, 34200}, // transition 3677
  {3526821000U, 37800}, // transition 3678
  {3542545800U, 34200}, // transition 3679
  {3558270600U, 37800}, // transition 3680
  {3573995400U, 34200}, // transition 3681
  {3589720200U, 37800}, // transition 3682
  {3605445000U, 34200}, // transition 3683
  {3621169800U, 37800}, // transition 3684
  {3636894600U, 34200}, // transition 3685
  {3653224200U, 37800}, // transition 3686
  {3668949000U, 34200}, // transition 3687
  {3684673800U, 37800}, // transition 3688
  {3700398600U, 34200}, // transition 3689
  {3716123400U, 37800}, // transition 3690
  {3731848200U, 34200}, // transition 3691
  {3747573000U, 37800}, // transition 3692
  {3763297800U, 34200}, // transition 3693
  {3779022600U, 37800}, // transition 3694
  {3794747400U, 34200}, // transition 3695
  {3810472200U, 37800}, // transition 3696
  {3826197000U, 34200}, // transition 3697
  {3842526600U, 37800}, // transition 3698
  {3858251400U, 34200}, // transition 3699
  {3873976200U, 37800}, // transition 3700
  {3889701000U, 34200}, // transition 3701
  {3905425800U, 37800}, // transition 3702
  {3921150600U, 34200}, // transition 3703
  {3936875400U, 37800}, // transition 3704
  {3952600200U, 34200}, // transition 3705
  {3968325000U, 37800}, // transition 3706
  {3984049800U, 34200}, // transition 3707
  {4000379400U, 37800}, // transition 3708
  {4016104200U, 34200}, // transition 3709
  {4031829000U, 37800}, // transition 3710
  {4047553800U, 34200}, // transition 3711
  {4063278600U, 37800}, // transition 3712
  {4079003400U, 34200}, // transition 3713
  {4094728200U, 37800}, // transition 3714
  {4110453000U, 34200}, // transition 3715
  {4126177800U, 37800}, // transition 3716
  {1767225600U, 34200}, // transition 3717
  {1767225600U, 31500}, // transition 3718
  {1767225600U, 39600}, // transition 3719
  {1775314800U, 37800}, // transition 3720
  {1791041400U, 39600}, // transition 3721
  {1806764400U, 37800}, // transition 3722
  {1822491000U, 39600}, // transition 3723
  {1838214000U, 37800}, // transition 3724
  {1853940600U, 39600}, // transition 3725
  {1869663600U, 37800}, // transition 3726
  {1885995000U, 39600}, // transition 3727
  {1901718000U, 37800}, // transition 3728
  {1917444600U, 39600}, // transition 3729
  {1933167600U, 37800}, // transition 3730
  {1948894200U, 39600}, // transition 3731
  {1964617200U, 37800}, // transition 3732
  {1980343800U, 39600}, // transition 3733
  {1996066800U, 37800}, // transition 3734
  {2011793400U, 39600}, // transition 3735
  {2027516400U, 37800}, // transition 3736
  {2043243000U, 39600}, // transition 3737
  {2058966000U, 37800}, // transition 3738
  {2075297400U, 39600}, // transition 3739
  {2091020400U, 37800}, // transition 3740
  {2106747000U, 39600}, // transition 3741
  {2122470000U, 37800}, // transition 3742
  {2138196600U, 39600}, // transition 3743
  {2153919600U, 37800}, // transition 3744
  {2169646200U, 39600}, // transition 3745
  {2185369200U, 37800}, // transition 3746
  {2201095800U, 39600}, // transition 3747
  {2216818800U, 37800}, // transition 3748
  {2233150200U, 39600}, // transition 3749
  {2248873200U, 37800}, // transition 3750
  {2264599800U, 39600}, // transition 3751
  {2280322800U, 37800}, // transition 3752
  {2296049400U, 39600}, // transition 3753
  {2311772400U, 37800}, // transition 3754
  {2327499000U, 39600}, // transition 3755
  {2343222000U, 37800}, // transition 3756
  {2358948600U, 39600}, // transition 3757
  {2374671600U, 37800}, // transition 3758
  {2390398200U, 39600}, // transition 3759
  {2406121200U, 37800}, // transition 3760
  {2422452600U, 39600}, // transition 3761
  {2438175600U, 37800}, // transition 3762
  {2453902200U, 39600}, // transition 3763
  {2469625200U, 37800}, // transition 3764
  {2485351800U, 39600}, // transition 3765
  {2501074800U, 37800}, // transition 3766
  {2516801400U, 39600}, // transition 3767
  {2532524400U, 37800}, // transition 3768
  {2548251000U, 39600}, // transition 3769
  {2563974000U, 37800}, // transition 3770
  {2579700600U, 39600}, // transition 3771
  {2596028400U, 37800}, // transition 3772
  {2611755000U, 39600}, // transition 3773
  {2627478000U, 37800}, // transition 3774
  {2643204600U, 39600}, // transition 3775
  {2658927600U, 37800}, // transition 3776
  {2674654200U, 39600}, // transition 3777
  {2690377200U, 37800}, // transition 3778
  {2706103800U, 39600}, // transition 3779
  {2721826800U, 37800}, // transition 3780
  {2737553400U, 39600}, // transition 3781
  {2753276400U, 37800}, // transition 3782
  {2769607800U, 39600}, // transition 3783
  {2785330800U, 37800}, // transition 3784
  {2801057400U, 39600}, // transition 3785
  {2816780400U, 37800}, // transition 3786
  {2832507000U, 39600}, // transition 3787
  {2848230000U, 37800}, // transition 3788
  {2863956600U, 39600}, // transition 3789
  {2879679600U, 37800}, // transition 3790
  {2895406200U, 39600}, // transition 3791
  {2911129200U, 37800}, // transition 3792
  {2926855800U, 39600}, // transition 3793
  {2942578800U, 37800}, // transition 3794
  {2958910200U, 39600}, // transition 3795
  {2974633200U, 37800}, // transition 3796
  {2990359800U, 39600}, // transition 3797
  {3006082800U, 37800}, // transition 3798
  {3021809400U, 39600}, // transition 3799
  {3037532400U, 37800}, // transition 3800
  {3053259000U, 39600}, // transition 3801
  {3068982000U, 37800}, // transition 3802
  {3084708600U, 39600}, // transition 3803
  {3100431600U, 37800}, // transition 3804
  {3116763000U, 39600}, // transition 3805
  {3132486000U, 37800}, // transition 3806
  {3148212600U, 39600}, // transition 3807
  {3163935600U, 37800}, // transition 3808
  {3179662200U, 39600}, // transition 3809
  {3195385200U, 37800}, // transition 3810
  {3211111800U, 39600}, // transition 3811
  {3226834800U, 37800}, // transition 3812
  {3242561400U, 39600}, // transition 3813
  {3258284400U, 37800}, // transition 3814
  {3274011000U, 39600}, // transition 3815
  {3289734000U, 37800}, // transition 3816
  {3306065400U, 39600}, // transition 3817
  {3321788400U, 37800}, // transition 3818
  {3337515000U, 39600}, // transition 3819
  {3353238000U, 37800}, // transition 3820
  {3368964600U, 39600}, // transition 3821
  {3384687600U, 37800}, // transition 3822
  {3400414200U, 39600}, // transition 3823
  {3416137200U, 37800}, // transition 3824
  {3431863800U, 39600}, // transition 3825
  {3447586800U, 37800}, // transition 3826
  {3463313400U, 39600}, // transition 3827
  {3479641200U, 37800}, // transition 3828
  {3495367800U, 39600}, // transition 3829
  {3511090800U, 37800}, // transition 3830
  {3526817400U, 39600}, // transition 3831
  {3542540400U, 37800}, // transition 3832
  {3558267000U, 39600}, // transition 3833
  {3573990000U, 37800}, // transition 3834
  {3589716600U, 39600}, // transition 3835
  {3605439600U, 37800}, // transition 3836
  {3621166200U, 39600}, // transition 3837
  {3636889200U, 37800}, // transition 3838
  {3653220600U, 39600}, // transition 3839
  {3668943600U, 37800}, // transition 3840
  {3684670200U, 39600}, // transition 3841
  {3700393200U, 37800}, // transition 3842
  {3716119800U, 39600}, // transition 3843
  {3731842800U, 37800}, // transition 3844
  {3747569400U, 39600}, // transition 3845
  {3763292400U, 37800}, // transition 3846
  {3779019000U, 39600}, // transition 3847
  {3794742000U, 37800}, // transition 3848
  {3810468600U, 39600}, // transition 3849
  {3826191600U, 37800}, // transition 3850
  {3842523000U, 39600}, // transition 3851
  {3858246000U, 37800}, // transition 3852
  {3873972600U, 39600}, // transition 3853
  {3889695600U, 37800}, // transition 3854
  {3905422200U, 39600}, // transition 3855
  {3921145200U, 37800}, // transition 3856
  {3936871800U, 39600}, // transition 3857
  {3952594800U, 37800}, // transition 3858
  {3968321400U, 39600}, // transition 3859
  {3984044400U, 37800}, // transition 3860
  {4000375800U, 39600}, // transition 3861
  {4016098800U, 37800}, // transition 3862
  {4031825400U, 39600}, // transition 3863
  {4047548400U, 37800}, // transition 3864
  {4063275000U, 39600}, // transition 3865
  {4078998000U, 37800}, // transition 3866
  {4094724600U, 39600}, // transition 3867
  {4110447600U, 37800}, // transition 3868
  {4126174200U, 39600}, // transition 3869
  {1767225600U, -18000}, // transition 3870
  {1775358000U, -21600}, // transition 3871
  {1788667200U, -18000}, // transition 3872
  {1806807600U, -21600}, // transition 3873
  {1820116800U, -18000}, // transition 3874
  {1838257200U, -21600}, // transition 3875
  {1851566400U, -18000}, // transition 3876
  {1870311600U, -21600}, // transition 3877
  {1883016000U, -18000}, // transition 3878
  {1901761200U, -21600}, // transition 3879
  {1915070400U, -18000}, // transition 3880
  {1933210800U, -21600}, // transition 3881
  {1946520000U, -18000}, // transition 3882
  {1964660400U, -21600}, // transition 3883
  {1977969600U, -18000}, // transition 3884
  {1996110000U, -21600}, // transition 3885
  {2009419200U, -18000}, // transition 3886
  {2027559600U, -21600}, // transition 3887
  {2040868800U, -18000}, // transition 3888
  {2059614000U, -21600}, // transition 3889
  {2072318400U, -18000}, // transition 3890
  {2091063600U, -21600}, // transition 3891
  {2104372800U, -18000}, // transition 3892
  {2122513200U, -21600}, // transition 3893
  {2135822400U, -18000}, // transition 3894
  {2153962800U, -21600}, // transition 3895
  {2167272000U, -18000}, // transition 3896
  {2185412400U, -21600}, // transition 3897
  {2198721600U, -18000}, // transition 3898
  {2217466800U, -21600}, // transition 3899
  {2230171200U, -18000}, // transition 3900
  {2248916400U, -21600}, // transition 3901
  {2262225600U, -18000}, // transition 3902
  {2280366000U, -21600}, // transition 3903
  {2293675200U, -18000}, // transition 3904
  {2311815600U, -21600}, // transition 3905
  {2325124800U, -18000}, // transition 3906
  {2343265200U, -21600}, // transition 3907
  {2356574400U, -18000}, // transition 3908
  {2374714800U, -21600}, // transition 3909
  {2388024000U, -18000}, // transition 3910
  {2406769200U, -21600}, // transition 3911
  {2419473600U, -18000}, // transition 3912
  {2438218800U, -21600}, // transition 3913
  {2451528000U, -18000}, // transition 3914
  {2469668400U, -21600}, // transition 3915
  {2482977600U, -18000}, // transition 3916
  {2501118000U, -21600}, // transition 3917
  {2514427200U, -18000}, // transition 3918
  {2532567600U, -21600}, // transition 3919
  {2545876800U, -18000}, // transition 3920
  {2564017200U, -21600}, // transition 3921
  {2577326400U, -18000}, // transition 3922
  {2596071600U, -21600}, // transition 3923
  {2609380800U, -18000}, // transition 3924
  {2627521200U, -21600}, // transition 3925
  {2640830400U, -18000}, // transition 3926
  {2658970800U, -21600}, // transition 3927
  {2672280000U, -18000}, // transition 3928
  {2690420400U, -21600}, // transition 3929
  {2703729600U, -18000}, // transition 3930
  {2721870000U, -21600}, // transition 3931
  {2735179200U, -18000}, // transition 3932
  {2753924400U, -21600}, // transition 3933
  {2766628800U, -18000}, // transition 3934
  {2785374000U, -21600}, // transition 3935
  {2798683200U, -18000}, // transition 3936
  {2816823600U, -21600}, // transition 3937
  {2830132800U, -18000}, // transition 3938
  {2848273200U, -21600}, // transition 3939
  {2861582400U, -18000}, // transition 3940
  {2879722800U, -21600}, // transition 3941
  {2893032000U, -18000}, // transition 3942
  {2911172400U, -21600}, // transition 3943
  {2924481600U, -18000}, // transition 3944
  {2943226800U, -21600}, // transition 3945
  {2955931200U, -18000}, // transition 3946
  {2974676400U, -21600}, // transition 3947
  {2987985600U, -18000}, // transition 3948
  {3006126000U, -21600}, // transition 3949
  {3019435200U, -18000}, // transition 3950
  {3037575600U, -21600}, // transition 3951
  {3050884800U, -18000}, // transition 3952
  {3069025200U, -21600}, // transition 3953
  {3082334400U, -18000}, // transition 3954
  {3101079600U, -21600}, // transition 3955
  {3113784000U, -18000}, // transition 3956
  {3132529200U, -21600}, // transition 3957
  {3145838400U, -18000}, // transition 3958
  {3163978800U, -21600}, // transition 3959
  {3177288000U, -18000}, // transition 3960
  {3195428400U, -21600}, // transition 3961
  {3208737600U, -18000}, // transition 3962
  {3226878000U, -21600}, // transition 3963
  {3240187200U, -18000}, // transition 3964
  {3258327600U, -21600}, // transition 3965
  {3271636800U, -18000}, // transition 3966
  {3290382000U, -21600}, // transition 3967
  {3303086400U, -18000}, // transition 3968
  {3321831600U, -21600}, // transition 3969
  {3335140800U, -18000}, // transition 3970
  {3353281200U, -21600}, // transition 3971
  {3366590400U, -18000}, // transition 3972
  {3384730800U, -21600}, // transition 3973
  {3398040000U, -18000}, // transition 3974
  {3416180400U, -21600}, // transition 3975
  {3429489600U, -18000}, // transition 3976
  {3447630000U, -21600}, // transition 3977
  {3460939200U, -18000}, // transition 3978
  {3479684400U, -21600}, // transition 3979
  {3492993600U, -18000}, // transition 3980
  {3511134000U, -21600}, // transition 3981
  {3524443200U, -18000}, // transition 3982
  {3542583600U, -21600}, // transition 3983
  {3555892800U, -18000}, // transition 3984
  {3574033200U, -21600}, // transition 3985
  {3587342400U, -18000}, // transition 3986
  {3605482800U, -21600}, // transition 3987
  {3618792000U, -18000}, // transition 3988
  {3637537200U, -21600}, // transition 3989
  {3650241600U, -18000}, // transition 3990
  {3668986800U, -21600}, // transition 3991
  {3682296000U, -18000}, // transition 3992
  {3700436400U, -21600}, // transition 3993
  {3713745600U, -18000}, // transition 3994
  {3731886000U, -21600}, // transition 3995
  {3745195200U, -18000}, // transition 3996
  {3763335600U, -21600}, // transition 3997
  {3776644800U, -18000}, // transition 3998
  {3794785200U, -21600}, // transition 3999
  {3808094400U, -18000}, // transition 4000
  {3826839600U, -21600}, // transition 4001
  {3839544000U, -18000}, // transition 4002
  {3858289200U, -21600}, // transition 4003
  {3871598400U, -18000}, // transition 4004
  {3889738800U, -21600}, // transition 4005
  {3903048000U, -18000}, // transition 4006
  {3921188400U, -21600}, // transition 4007
  {3934497600U, -18000}, // transition 4008
  {3952638000U, -21600}, // transition 4009
  {3965947200U, -18000}, // transition 4010
  {3984692400U, -21600}, // transition 4011
  {3997396800U, -18000}, // transition 4012
  {4016142000U, -21600}, // transition 4013
  {4029451200U, -18000}, // transition 4014
  {4047591600U, -21600}, // transition 4015
  {4060900800U, -18000}, // transition 4016
  {4079041200U, -21600}, // transition 4017
  {4092350400U, -18000}, // transition 4018
  {4110490800U, -21600}, // transition 4019
  {4123800000U, -18000}, // transition 4020
  {1767225600U, -36000}, // transition 4021
  {1767225600U, -39600}, // transition 4022
  {1767225600U, -43200}, // transition 4023
  {1767225600U, -28800}, // transition 4024
  {1767225600U, -32400}, // transition 4025
  {1767225600U, 46800}, // transition 4026
  {1767225600U, 50400}, // transition 4027
  {1767225600U, 49500}, // transition 4028
  {1775311200U, 45900}, // transition 4029
  {1790431200U, 49500}, // transition 4030
  {1806760800U, 45900}, // transition 4031
  {1821880800U, 49500}, // transition 4032
  {1838210400U, 45900}, // transition 4033
  {1853330400U, 49500}, // transition 4034
  {1869660000U, 45900}, // transition 4035
  {1885384800U, 49500}, // transition 4036
  {1901714400U, 45900}, // transition 4037
  {1916834400U, 49500}, // transition 4038
  {1933164000U, 45900}, // transition 4039
  {1948284000U, 49500}, // transition 4040
  {1964613600U, 45900}, // transition 4041
  {1979733600U, 49500}, // transition 4042
  {1996063200U, 45900}, // transition 4043
  {2011183200U, 49500}, // transition 4044
  {2027512800U, 45900}, // transition 4045
  {2042632800U, 49500}, // transition 4046
  {2058962400U, 45900}, // transition 4047
  {2074687200U, 49500}, // transition 4048
  {2091016800U, 45900}, // transition 4049
  {2106136800U, 49500}, // transition 4050
  {2122466400U, 45900}, // transition 4051
  {2137586400U, 49500}, // transition 4052
  {2153916000U, 45900}, // transition 4053
  {2169036000U, 49500}, // transition 4054
  {2185365600U, 45900}, // transition 4055
  {2200485600U, 49500}, // transition 4056
  {2216815200U, 45900}, // transition 4057
  {2232540000U, 49500}, // transition 4058
  {2248869600U, 45900}, // transition 4059
  {2263989600U, 49500}, // transition 4060
  {2280319200U, 45900}, // transition 4061
  {2295439200U, 49500}, // transition 4062
  {2311768800U, 45900}, // transition 4063
  {2326888800U, 49500}, // transition 4064
  {2343218400U, 45900}, // transition 4065
  {2358338400U, 49500}, // transition 4066
  {2374668000U, 45900}, // transition 4067
  {2389788000U, 49500}, // transition 4068
  {2406117600U, 45900}, // transition 4069
  {2421842400U, 49500}, // transition 4070
  {2438172000U, 45900}, // transition 4071
  {2453292000U, 49500}, // transition 4072
  {2469621600U, 45900}, // transition 4073
  {2484741600U, 49500}, // transition 4074
  {2501071200U, 45900}, // transition 4075
  {2516191200U, 49500}, // transition 4076
  {2532520800U, 45900}, // transition 4077
  {2547640800U, 49500}, // transition 4078
  {2563970400U, 45900}, // transition 4079
  {2579090400U, 49500}, // transition 4080
  {2596024800U, 45900}, // transition 4081
  {2611144800U, 49500}, // transition 4082
  {2627474400U, 45900}, // transition 4083
  {2642594400U, 49500}, // transition 4084
  {2658924000U, 45900}, // transition 4085
  {2674044000U, 49500}, // transition 4086
  {2690373600U, 45900}, // transition 4087
  {2705493600U, 49500}, // transition 4088
  {2721823200U, 45900}, // transition 4089
  {2736943200U, 49500}, // transition 4090
  {2753272800U, 45900}, // transition 4091
  {2768997600U, 49500}, // transition 4092
  {2785327200U, 45900}, // transition 4093
  {2800447200U, 49500}, // transition 4094
  {2816776800U, 45900}, // transition 4095
  {2831896800U, 49500}, // transition 4096
  {2848226400U, 45900}, // transition 4097
  {2863346400U, 49500}, // transition 4098
  {2879676000U, 45900}, // transition 4099
  {2894796000U, 49500}, // transition 4100
  {2911125600U, 45900}, // transition 4101
  {2926245600U, 49500}, // transition 4102
  {2942575200U, 45900}, // transition 4103
  {2958300000U, 49500}, // transition 4104
  {2974629600U, 45900}, // transition 4105
  {2989749600U, 49500}, // transition 4106
  {3006079200U, 45900}, // transition 4107
  {3021199200U, 49500}, // transition 4108
  {3037528800U, 45900}, // transition 4109
  {3052648800U, 49500}, // transition 4110
  {3068978400U, 45900}, // transition 4111
  {3084098400U, 49500}, // transition 4112
  {3100428000U, 45900}, // transition 4113
  {3116152800U, 49500}, // transition 4114
  {3132482400U, 45900}, // transition 4115
  {3147602400U, 49500}, // transition 4116
  {3163932000U, 45900}, // transition 4117
  {3179052000U, 49500}, // transition 4118
  {3195381600U, 45900}, // transition 4119
  {3210501600U, 49500}, // transition 4120
  {3226831200U, 45900}, // transition 4121
  {3241951200U, 49500}, // transition 4122
  {3258280800U, 45900}, // transition 4123
  {3273400800U, 49500}, // transition 4124
  {3289730400U, 45900}, // transition 4125
  {3305455200U, 49500}, // transition 4126
  {3321784800U, 45900}, // transition 4127
  {3336904800U, 49500}, // transition 4128
  {3353234400U, 45900}, // transition 4129
  {3368354400U, 49500}, // transition 4130
  {3384684000U, 45900}, // transition 4131
  {3399804000U, 49500}, // transition 4132
  {3416133600U, 45900}, // transition 4133
  {3431253600U, 49500}, // transition 4134
  {3447583200U, 45900}, // transition 4135
  {3462703200U, 49500}, // transition 4136
  {3479637600U, 45900}, // transition 4137
  {3494757600U, 49500}, // transition 4138
  {3511087200U, 45900}, // transition 4139
  {3526207200U, 49500}, // transition 4140
  {3542536800U, 45900}, // transition 4141
  {3557656800U, 49500}, // transition 4142
  {3573986400U, 45900}, // transition 4143
  {3589106400U, 49500}, // transition 4144
  {3605436000U, 45900}, // transition 4145
  {3620556000U, 49500}, // transition 4146
  {3636885600U, 45900}, // transition 4147
  {3652610400U, 49500}, // transition 4148
  {3668940000U, 45900}, // transition 4149
  {3684060000U, 49500}, // transition 4150
  {3700389600U, 45900}, // transition 4151
  {3715509600U, 49500}, // transition 4152
  {3731839200U, 45900}, // transition 4153
  {3746959200U, 49500}, // transition 4154
  {3763288800U, 45900}, // transition 4155
  {3778408800U, 49500}, // transition 4156
  {3794738400U, 45900}, // transition 4157
  {3809858400U, 49500}, // transition 4158
  {3826188000U, 45900}, // transition 4159
  {3841912800U, 49500}, // transition 4160
  {3858242400U, 45900}, // transition 4161
  {3873362400U, 49500}, // transition 4162
  {3889692000U, 45900}, // transition 4163
  {3904812000U, 49500}, // transition 4164
  {3921141600U, 45900}, // transition 4165
  {3936261600U, 49500}, // transition 4166
  {3952591200U, 45900}, // transition 4167
  {3967711200U, 49500}, // transition 4168
  {3984040800U, 45900}, // transition 4169
  {3999765600U, 49500}, // transition 4170
  {4016095200U, 45900}, // transition 4171
  {4031215200U, 49500}, // transition 4172
  {4047544800U, 45900}, // transition 4173
  {4062664800U, 49500}, // transition 4174
  {4078994400U, 45900}, // transition 4175
  {4094114400U, 49500}, // transition 4176
  {4110444000U, 45900}, // transition 4177
  {4125564000U, 49500}, // transition 4178
  {1767225600U, -34200}, // transition 4179
  {1767225600U, 43200}, // transition 4180
  {1775314800U, 39600}, // transition 4181
  {1791039600U, 43200}, // transition 4182
  {1806764400U, 39600}, // transition 4183
  {1822489200U, 43200}, // transition 4184
  {1838214000U, 39600}, // transition 4185
  {1853938800U, 43200}, // transition 4186
  {1869663600U, 39600}, // transition 4187
  {1885993200U, 43200}, // transition 4188
  {1901718000U, 39600}, // transition 4189
  {1917442800U, 43200}, // transition 4190
  {1933167600U, 39600}, // transition 4191
  {1948892400U, 43200}, // transition 4192
  {1964617200U, 39600}, // transition 4193
  {1980342000U, 43200}, // transition 4194
  {1996066800U, 39600}, // transition 4195
  {2011791600U, 43200}, // transition 4196
  {2027516400U, 39600}, // transition 4197
  {2043241200U, 43200}, // transition 4198
  {2058966000U, 39600}, // transition 4199
  {2075295600U, 43200}, // transition 4200
  {2091020400U, 39600}, // transition 4201
  {2106745200U, 43200}, // transition 4202
  {2122470000U, 39600}, // transition 4203
  {2138194800U, 43200}, // transition 4204
  {2153919600U, 39600}, // transition 4205
  {2169644400U, 43200}, // transition 4206
  {2185369200U, 39600}, // transition 4207
  {2201094000U, 43200}, // transition 4208
  {2216818800U, 39600}, // transition 4209
  {2233148400U, 43200}, // transition 4210
  {2248873200U, 39600}, // transition 4211
  {2264598000U, 43200}, // transition 4212
  {2280322800U, 39600}, // transition 4213
  {2296047600U, 43200}, // transition 4214
  {2311772400U, 39600}, // transition 4215
  {2327497200U, 43200}, // transition 4216
  {2343222000U, 39600}, // transition 4217
  {2358946800U, 43200}, // transition 4218
  {2374671600U, 39600}, // transition 4219
  {2390396400U, 43200}, // transition 4220
  {2406121200U, 39600}, // transition 4221
  {2422450800U, 43200}, // transition 4222
  {2438175600U, 39600}, // transition 4223
  {2453900400U, 43200}, // transition 4224
  {2469625200U, 39600}, // transition 4225
  {2485350000U, 43200}, // transition 4226
  {2501074800U, 39600}, // transition 4227
  {2516799600U, 43200}, // transition 4228
  {2532524400U, 39600}, // transition 4229
  {2548249200U, 43200}, // transition 4230
  {2563974000U, 39600}, // transition 4231
  {2579698800U, 43200}, // transition 4232
  {2596028400U, 39600}, // transition 4233
  {2611753200U, 43200}, // transition 4234
  {2627478000U, 39600}, // transition 4235
  {2643202800U, 43200}, // transition 4236
  {2658927600U, 39600}, // transition 4237
  {2674652400U, 43200}, // transition 4238
  {2690377200U, 39600}, // transition 4239
  {2706102000U, 43200}, // transition 4240
  {2721826800U, 39600}, // transition 4241
  {2737551600U, 43200}, // transition 4242
  {2753276400U, 39600}, // transition 4243
  {2769606000U, 43200}, // transition 4244
  {2785330800U, 39600}, // transition 4245
  {2801055600U, 43200}, // transition 4246
  {2816780400U, 39600}, // transition 4247
  {2832505200U, 43200}, // transition 4248
  {2848230000U, 39600}, // transition 4249
  {2863954800U, 43200}, // transition 4250
  {2879679600U, 39600}, // transition 4251
  {2895404400U, 43200}, // transition 4252
  {2911129200U, 39600}, // transition 4253
  {2926854000U, 43200}, // transition 4254
  {2942578800U, 39600}, // transition 4255
  {2958908400U, 43200}, // transition 4256
  {2974633200U, 39600}, // transition 4257
  {2990358000U, 43200}, // transition 4258
  {3006082800U, 39600}, // transition 4259
  {3021807600U, 43200}, // transition 4260
  {3037532400U, 39600}, // transition 4261
  {3053257200U, 43200}, // transition 4262
  {3068982000U, 39600}, // transition 4263
  {3084706800U, 43200}, // transition 4264
  {3100431600U, 39600}, // transition 4265
  {3116761200U, 43200}, // transition 4266
  {3132486000U, 39600}, // transition 4267
  {3148210800U, 43200}, // transition 4268
  {3163935600U, 39600}, // transition 4269
  {3179660400U, 43200}, // transition 4270
  {3195385200U, 39600}, // transition 4271
  {3211110000U, 43200}, // transition 4272
  {3226834800U, 39600}, // transition 4273
  {3242559600U, 43200}, // transition 4274
  {3258284400U, 39600}, // transition 4275
  {3274009200U, 43200}, // transition 4276
  {3289734000U, 39600}, // transition 4277
  {3306063600U, 43200}, // transition 4278
  {3321788400U, 39600}, // transition 4279
  {3337513200U, 43200}, // transition 4280
  {3353238000U, 39600}, // transition 4281
  {3368962800U, 43200}, // transition 4282
  {3384687600U, 39600}, // transition 4283
  {3400412400U, 43200}, // transition 4284
  {3416137200U, 39600}, // transition 4285
  {3431862000U, 43200}, // transition 4286
  {3447586800U, 39600}, // transition 4287
  {3463311600U, 43200}, // transition 4288
  {3479641200U, 39600}, // transition 4289
  {3495366000U, 43200}, // transition 4290
  {3511090800U, 39600}, // transition 4291
  {3526815600U, 43200}, // transition 4292
  {3542540400U, 39600}, // transition 4293
  {3558265200U, 43200}, // transition 4294
  {3573990000U, 39600}, // transition 4295
  {3589714800U, 43200}, // transition 4296
  {3605439600U, 39600}, // transition 4297
  {3621164400U, 43200}, // transition 4298
  {3636889200U, 39600}, // transition 4299
  {3653218800U, 43200}, // transition 4300
  {3668943600U, 39600}, // transition 4301
  {3684668400U, 43200}, // transition 4302
  {3700393200U, 39600}, // transition 4303
  {3716118000U, 43200}, // transition 4304
  {3731842800U, 39600}, // transition 4305
  {3747567600U, 43200}, // transition 4306
  {3763292400U, 39600}, // transition 4307
  {3779017200U, 43200}, // transition 4308
  {3794742000U, 39600}, // transition 4309
  {3810466800U, 43200}, // transition 4310
  {3826191600U, 39600}, // transition 4311
  {3842521200U, 43200}, // transition 4312
  {3858246000U, 39600}, // transition 4313
  {3873970800U, 43200}, // transition 4314
  {3889695600U, 39600}, // transition 4315
  {3905420400U, 43200}, // transition 4316
  {3921145200U, 39600}, // transition 4317
  {3936870000U, 43200}, // transition 4318
  {3952594800U, 39600}, // transition 4319
  {3968319600U, 43200}, // transition 4320
  {3984044400U, 39600}, // transition 4321
  {4000374000U, 43200}, // transition 4322
  {4016098800U, 39600}, // transition 4323
  {4031823600U, 43200}, // transition 4324
  {4047548400U, 39600}, // transition 4325
  {4063273200U, 43200}, // transition 4326
  {4078998000U, 39600}, // transition 4327
  {4094722800U, 43200}, // transition 4328
  {4110447600U, 39600}, // transition 4329
  {4126172400U, 43200}, // transition 4330
};
const HunterTimezone HUNTER_TIMEZONES[] = {
  {"Africa/Abidjan", 0, 1}, // zone 0
  {"Africa/Accra", 0, 1}, // zone 1
  {"Africa/Addis_Ababa", 1, 1}, // zone 2
  {"Africa/Algiers", 2, 1}, // zone 3
  {"Africa/Asmara", 1, 1}, // zone 4
  {"Africa/Asmera", 1, 1}, // zone 5
  {"Africa/Bamako", 0, 1}, // zone 6
  {"Africa/Bangui", 2, 1}, // zone 7
  {"Africa/Banjul", 0, 1}, // zone 8
  {"Africa/Bissau", 0, 1}, // zone 9
  {"Africa/Blantyre", 3, 1}, // zone 10
  {"Africa/Brazzaville", 2, 1}, // zone 11
  {"Africa/Bujumbura", 3, 1}, // zone 12
  {"Africa/Cairo", 4, 151}, // zone 13
  {"Africa/Casablanca", 155, 4}, // zone 14
  {"Africa/Ceuta", 159, 151}, // zone 15
  {"Africa/Conakry", 0, 1}, // zone 16
  {"Africa/Dakar", 0, 1}, // zone 17
  {"Africa/Dar_es_Salaam", 1, 1}, // zone 18
  {"Africa/Djibouti", 1, 1}, // zone 19
  {"Africa/Douala", 2, 1}, // zone 20
  {"Africa/El_Aaiun", 155, 4}, // zone 21
  {"Africa/Freetown", 0, 1}, // zone 22
  {"Africa/Gaborone", 3, 1}, // zone 23
  {"Africa/Harare", 3, 1}, // zone 24
  {"Africa/Johannesburg", 3, 1}, // zone 25
  {"Africa/Juba", 3, 1}, // zone 26
  {"Africa/Kampala", 1, 1}, // zone 27
  {"Africa/Khartoum", 3, 1}, // zone 28
  {"Africa/Kigali", 3, 1}, // zone 29
  {"Africa/Kinshasa", 2, 1}, // zone 30
  {"Africa/Lagos", 2, 1}, // zone 31
  {"Africa/Libreville", 2, 1}, // zone 32
  {"Africa/Lome", 0, 1}, // zone 33
  {"Africa/Luanda", 2, 1}, // zone 34
  {"Africa/Lubumbashi", 3, 1}, // zone 35
  {"Africa/Lusaka", 3, 1}, // zone 36
  {"Africa/Malabo", 2, 1}, // zone 37
  {"Africa/Maputo", 3, 1}, // zone 38
  {"Africa/Maseru", 3, 1}, // zone 39
  {"Africa/Mbabane", 3, 1}, // zone 40
  {"Africa/Mogadishu", 1, 1}, // zone 41
  {"Africa/Monrovia", 0, 1}, // zone 42
  {"Africa/Nairobi", 1, 1}, // zone 43
  {"Africa/Ndjamena", 2, 1}, // zone 44
  {"Africa/Niamey", 2, 1}, // zone 45
  {"Africa/Nouakchott", 0, 1}, // zone 46
  {"Africa/Ouagadougou", 0, 1}, // zone 47
  {"Africa/Porto-Novo", 2, 1}, // zone 48
  {"Africa/Sao_Tome", 0, 1}, // zone 49
  {"Africa/Timbuktu", 0, 1}, // zone 50
  {"Africa/Tripoli", 3, 1}, // zone 51
  {"Africa/Tunis", 2, 1}, // zone 52
  {"Africa/Windhoek", 3, 1}, // zone 53
  {"America/Adak", 310, 151}, // zone 54
  {"America/Anchorage", 461, 151}, // zone 55
  {"America/Anguilla", 612, 1}, // zone 56
  {"America/Antigua", 612, 1}, // zone 57
  {"America/Araguaina", 613, 1}, // zone 58
  {"America/Argentina/Buenos_Aires", 613, 1}, // zone 59
  {"America/Argentina/Catamarca", 613, 1}, // zone 60
  {"America/Argentina/ComodRivadavia", 613, 1}, // zone 61
  {"America/Argentina/Cordoba", 613, 1}, // zone 62
  {"America/Argentina/Jujuy", 613, 1}, // zone 63
  {"America/Argentina/La_Rioja", 613, 1}, // zone 64
  {"America/Argentina/Mendoza", 613, 1}, // zone 65
  {"America/Argentina/Rio_Gallegos", 613, 1}, // zone 66
  {"America/Argentina/Salta", 613, 1}, // zone 67
  {"America/Argentina/San_Juan", 613, 1}, // zone 68
  {"America/Argentina/San_Luis", 613, 1}, // zone 69
  {"America/Argentina/Tucuman", 613, 1}, // zone 70
  {"America/Argentina/Ushuaia", 613, 1}, // zone 71
  {"America/Aruba", 612, 1}, // zone 72
  {"America/Asuncion", 613, 1}, // zone 73
  {"America/Atikokan", 614, 1}, // zone 74
  {"America/Atka", 310, 151}, // zone 75
  {"America/Bahia", 613, 1}, // zone 76
  {"America/Bahia_Banderas", 615, 1}, // zone 77
  {"America/Barbados", 612, 1}, // zone 78
  {"America/Belem", 613, 1}, // zone 79
  {"America/Belize", 615, 1}, // zone 80
  {"America/Blanc-Sablon", 612, 1}, // zone 81
  {"America/Boa_Vista", 612, 1}, // zone 82
  {"America/Bogota", 614, 1}, // zone 83
  {"America/Boise", 616, 151}, // zone 84
  {"America/Buenos_Aires", 613, 1}, // zone 85
  {"America/Cambridge_Bay", 616, 151}, // zone 86
  {"America/Campo_Grande", 612, 1}, // zone 87
  {"America/Cancun", 614, 1}, // zone 88
  {"America/Caracas", 612, 1}, // zone 89
  {"America/Catamarca", 613, 1}, // zone 90
  {"America/Cayenne", 613, 1}, // zone 91
  {"America/Cayman", 614, 1}, // zone 92
  {"America/Chicago", 767, 151}, // zone 93
  {"America/Chihuahua", 615, 1}, // zone 94
  {"America/Ciudad_Juarez", 616, 151}, // zone 95
  {"America/Coral_Harbour", 614, 1}, // zone 96
  {"America/Cordoba", 613, 1}, // zone 97
  {"America/Costa_Rica", 615, 1}, // zone 98
  {"America/Coyhaique", 613, 1}, // zone 99
  {"America/Creston", 918, 1}, // zone 100
  {"America/Cuiaba", 612, 1}, // zone 101
  {"America/Curacao", 612, 1}, // zone 102
  {"America/Danmarkshavn", 0, 1}, // zone 103
  {"America/Dawson", 918, 1}, // zone 104
  {"America/Dawson_Creek", 918, 1}, // zone 105
  {"America/Denver", 616, 151}, // zone 106
  {"America/Detroit", 919, 151}, // zone 107
  {"America/Dominica", 612, 1}, // zone 108
  {"America/Edmonton", 1070, 2}, // zone 109
  {"America/Eirunepe", 614, 1}, // zone 110
  {"America/El_Salvador", 615, 1}, // zone 111
  {"America/Ensenada", 1072, 151}, // zone 112
  {"America/Fort_Nelson", 918, 1}, // zone 113
  {"America/Fort_Wayne", 919, 151}, // zone 114
  {"America/Fortaleza", 613, 1}, // zone 115
  {"America/Glace_Bay", 1223, 151}, // zone 116
  {"America/Godthab", 1374, 151}, // zone 117
  {"America/Goose_Bay", 1223, 151}, // zone 118
  {"America/Grand_Turk", 919, 151}, // zone 119
  {"America/Grenada", 612, 1}, // zone 120
  {"America/Guadeloupe", 612, 1}, // zone 121
  {"America/Guatemala", 615, 1}, // zone 122
  {"America/Guayaquil", 614, 1}, // zone 123
  {"America/Guyana", 612, 1}, // zone 124
  {"America/Halifax", 1223, 151}, // zone 125
  {"America/Havana", 1525, 149}, // zone 126
  {"America/Hermosillo", 918, 1}, // zone 127
  {"America/Indiana/Indianapolis", 919, 151}, // zone 128
  {"America/Indiana/Knox", 767, 151}, // zone 129
  {"America/Indiana/Marengo", 919, 151}, // zone 130
  {"America/Indiana/Petersburg", 919, 151}, // zone 131
  {"America/Indiana/Tell_City", 767, 151}, // zone 132
  {"America/Indiana/Vevay", 919, 151}, // zone 133
  {"America/Indiana/Vincennes", 919, 151}, // zone 134
  {"America/Indiana/Winamac", 919, 151}, // zone 135
  {"America/Indianapolis", 919, 151}, // zone 136
  {"America/Inuvik", 1070, 2}, // zone 137
  {"America/Iqaluit", 919, 151}, // zone 138
  {"America/Jamaica", 614, 1}, // zone 139
  {"America/Jujuy", 613, 1}, // zone 140
  {"America/Juneau", 461, 151}, // zone 141
  {"America/Kentucky/Louisville", 919, 151}, // zone 142
  {"America/Kentucky/Monticello", 919, 151}, // zone 143
  {"America/Knox_IN", 767, 151}, // zone 144
  {"America/Kralendijk", 612, 1}, // zone 145
  {"America/La_Paz", 612, 1}, // zone 146
  {"America/Lima", 614, 1}, // zone 147
  {"America/Los_Angeles", 1072, 151}, // zone 148
  {"America/Louisville", 919, 151}, // zone 149
  {"America/Lower_Princes", 612, 1}, // zone 150
  {"America/Maceio", 613, 1}, // zone 151
  {"America/Managua", 615, 1}, // zone 152
  {"America/Manaus", 612, 1}, // zone 153
  {"America/Marigot", 612, 1}, // zone 154
  {"America/Martinique", 612, 1}, // zone 155
  {"America/Matamoros", 767, 151}, // zone 156
  {"America/Mazatlan", 918, 1}, // zone 157
  {"America/Mendoza", 613, 1}, // zone 158
  {"America/Menominee", 767, 151}, // zone 159
  {"America/Merida", 615, 1}, // zone 160
  {"America/Metlakatla", 461, 151}, // zone 161
  {"America/Mexico_City", 615, 1}, // zone 162
  {"America/Miquelon", 1674, 151}, // zone 163
  {"America/Moncton", 1223, 151}, // zone 164
  {"America/Monterrey", 615, 1}, // zone 165
  {"America/Montevideo", 613, 1}, // zone 166
  {"America/Montreal", 919, 151}, // zone 167
  {"America/Montserrat", 612, 1}, // zone 168
  {"America/Nassau", 919, 151}, // zone 169
  {"America/New_York", 919, 151}, // zone 170
  {"America/Nipigon", 919, 151}, // zone 171
  {"America/Nome", 461, 151}, // zone 172
  {"America/Noronha", 1825, 1}, // zone 173
  {"America/North_Dakota/Beulah", 767, 151}, // zone 174
  {"America/North_Dakota/Center", 767, 151}, // zone 175
  {"America/North_Dakota/New_Salem", 767, 151}, // zone 176
  {"America/Nuuk", 1374, 151}, // zone 177
  {"America/Ojinaga", 767, 151}, // zone 178
  {"America/Panama", 614, 1}, // zone 179
  {"America/Pangnirtung", 919, 151}, // zone 180
  {"America/Paramaribo", 613, 1}, // zone 181
  {"America/Phoenix", 918, 1}, // zone 182
  {"America/Port-au-Prince", 919, 151}, // zone 183
  {"America/Port_of_Spain", 612, 1}, // zone 184
  {"America/Porto_Acre", 614, 1}, // zone 185
  {"America/Porto_Velho", 612, 1}, // zone 186
  {"America/Puerto_Rico", 612, 1}, // zone 187
  {"America/Punta_Arenas", 613, 1}, // zone 188
  {"America/Rainy_River", 1826, 2}, // zone 189
  {"America/Rankin_Inlet", 767, 151}, // zone 190
  {"America/Recife", 613, 1}, // zone 191
  {"America/Regina", 615, 1}, // zone 192
  {"America/Resolute", 767, 151}, // zone 193
  {"America/Rio_Branco", 614, 1}, // zone 194
  {"America/Rosario", 613, 1}, // zone 195
  {"America/Santa_Isabel", 1072, 151}, // zone 196
  {"America/Santarem", 613, 1}, // zone 197
  {"America/Santiago", 1828, 151}, // zone 198
  {"America/Santo_Domingo", 612, 1}, // zone 199
  {"America/Sao_Paulo", 613, 1}, // zone 200
  {"America/Scoresbysund", 1374, 151}, // zone 201
  {"America/Shiprock", 616, 151}, // zone 202
  {"America/Sitka", 461, 151}, // zone 203
  {"America/St_Barthelemy", 612, 1}, // zone 204
  {"America/St_Johns", 1979, 151}, // zone 205
  {"America/St_Kitts", 612, 1}, // zone 206
  {"America/St_Lucia", 612, 1}, // zone 207
  {"America/St_Thomas", 612, 1}, // zone 208
  {"America/St_Vincent", 612, 1}, // zone 209
  {"America/Swift_Current", 615, 1}, // zone 210
  {"America/Tegucigalpa", 615, 1}, // zone 211
  {"America/Thule", 1223, 151}, // zone 212
  {"America/Thunder_Bay", 919, 151}, // zone 213
  {"America/Tijuana", 1072, 151}, // zone 214
  {"America/Toronto", 919, 151}, // zone 215
  {"America/Tortola", 612, 1}, // zone 216
  {"America/Vancouver", 2130, 2}, // zone 217
  {"America/Virgin", 612, 1}, // zone 218
  {"America/Whitehorse", 918, 1}, // zone 219
  {"America/Winnipeg", 1826, 2}, // zone 220
  {"America/Yakutat", 461, 151}, // zone 221
  {"America/Yellowknife", 1070, 2}, // zone 222
  {"Antarctica/Casey", 2132, 1}, // zone 223
  {"Antarctica/Davis", 2133, 1}, // zone 224
  {"Antarctica/DumontDUrville", 2134, 1}, // zone 225
  {"Antarctica/Macquarie", 2135, 151}, // zone 226
  {"Antarctica/Mawson", 2286, 1}, // zone 227
  {"Antarctica/McMurdo", 2287, 151}, // zone 228
  {"Antarctica/Palmer", 613, 1}, // zone 229
  {"Antarctica/Rothera", 613, 1}, // zone 230
  {"Antarctica/South_Pole", 2287, 151}, // zone 231
  {"Antarctica/Syowa", 1, 1}, // zone 232
  {"Antarctica/Troll", 2438, 151}, // zone 233
  {"Antarctica/Vostok", 2286, 1}, // zone 234
  {"Arctic/Longyearbyen", 159, 151}, // zone 235
  {"Asia/Aden", 1, 1}, // zone 236
  {"Asia/Almaty", 2286, 1}, // zone 237
  {"Asia/Amman", 1, 1}, // zone 238
  {"Asia/Anadyr", 2589, 1}, // zone 239
  {"Asia/Aqtau", 2286, 1}, // zone 240
  {"Asia/Aqtobe", 2286, 1}, // zone 241
  {"Asia/Ashgabat", 2286, 1}, // zone 242
  {"Asia/Ashkhabad", 2286, 1}, // zone 243
  {"Asia/Atyrau", 2286, 1}, // zone 244
  {"Asia/Baghdad", 1, 1}, // zone 245
  {"Asia/Bahrain", 1, 1}, // zone 246
  {"Asia/Baku", 2590, 1}, // zone 247
  {"Asia/Bangkok", 2133, 1}, // zone 248
  {"Asia/Barnaul", 2133, 1}, // zone 249
  {"Asia/Beirut", 2591, 151}, // zone 250
  {"Asia/Bishkek", 2742, 1}, // zone 251
  {"Asia/Brunei", 2132, 1}, // zone 252
  {"Asia/Calcutta", 2743, 1}, // zone 253
  {"Asia/Chita", 2744, 1}, // zone 254
  {"Asia/Choibalsan", 2132, 1}, // zone 255
  {"Asia/Chongqing", 2132, 1}, // zone 256
  {"Asia/Chungking", 2132, 1}, // zone 257
  {"Asia/Colombo", 2743, 1}, // zone 258
  {"Asia/Dacca", 2742, 1}, // zone 259
  {"Asia/Damascus", 1, 1}, // zone 260
  {"Asia/Dhaka", 2742, 1}, // zone 261
  {"Asia/Dili", 2744, 1}, // zone 262
  {"Asia/Dubai", 2590, 1}, // zone 263
  {"Asia/Dushanbe", 2286, 1}, // zone 264
  {"Asia/Famagusta", 2745, 151}, // zone 265
  {"Asia/Gaza", 2896, 211}, // zone 266
  {"Asia/Harbin", 2132, 1}, // zone 267
  {"Asia/Hebron", 2896, 211}, // zone 268
  {"Asia/Ho_Chi_Minh", 2133, 1}, // zone 269
  {"Asia/Hong_Kong", 2132, 1}, // zone 270
  {"Asia/Hovd", 2133, 1}, // zone 271
  {"Asia/Irkutsk", 2132, 1}, // zone 272
  {"Asia/Istanbul", 1, 1}, // zone 273
  {"Asia/Jakarta", 2133, 1}, // zone 274
  {"Asia/Jayapura", 2744, 1}, // zone 275
  {"Asia/Jerusalem", 3107, 151}, // zone 276
  {"Asia/Kabul", 3258, 1}, // zone 277
  {"Asia/Kamchatka", 2589, 1}, // zone 278
  {"Asia/Karachi", 2286, 1}, // zone 279
  {"Asia/Kashgar", 2742, 1}, // zone 280
  {"Asia/Kathmandu", 3259, 1}, // zone 281
  {"Asia/Katmandu", 3259, 1}, // zone 282
  {"Asia/Khandyga", 2744, 1}, // zone 283
  {"Asia/Kolkata", 2743, 1}, // zone 284
  {"Asia/Krasnoyarsk", 2133, 1}, // zone 285
  {"Asia/Kuala_Lumpur", 2132, 1}, // zone 286
  {"Asia/Kuching", 2132, 1}, // zone 287
  {"Asia/Kuwait", 1, 1}, // zone 288
  {"Asia/Macao", 2132, 1}, // zone 289
  {"Asia/Macau", 2132, 1}, // zone 290
  {"Asia/Magadan", 3260, 1}, // zone 291
  {"Asia/Makassar", 2132, 1}, // zone 292
  {"Asia/Manila", 2132, 1}, // zone 293
  {"Asia/Muscat", 2590, 1}, // zone 294
  {"Asia/Nicosia", 2745, 151}, // zone 295
  {"Asia/Novokuznetsk", 2133, 1}, // zone 296
  {"Asia/Novosibirsk", 2133, 1}, // zone 297
  {"Asia/Omsk", 2742, 1}, // zone 298
  {"Asia/Oral", 2286, 1}, // zone 299
  {"Asia/Phnom_Penh", 2133, 1}, // zone 300
  {"Asia/Pontianak", 2133, 1}, // zone 301
  {"Asia/Pyongyang", 2744, 1}, // zone 302
  {"Asia/Qatar", 1, 1}, // zone 303
  {"Asia/Qostanay", 2286, 1}, // zone 304
  {"Asia/Qyzylorda", 2286, 1}, // zone 305
  {"Asia/Rangoon", 3261, 1}, // zone 306
  {"Asia/Riyadh", 1, 1}, // zone 307
  {"Asia/Saigon", 2133, 1}, // zone 308
  {"Asia/Sakhalin", 3260, 1}, // zone 309
  {"Asia/Samarkand", 2286, 1}, // zone 310
  {"Asia/Seoul", 2744, 1}, // zone 311
  {"Asia/Shanghai", 2132, 1}, // zone 312
  {"Asia/Singapore", 2132, 1}, // zone 313
  {"Asia/Srednekolymsk", 3260, 1}, // zone 314
  {"Asia/Taipei", 2132, 1}, // zone 315
  {"Asia/Tashkent", 2286, 1}, // zone 316
  {"Asia/Tbilisi", 2590, 1}, // zone 317
  {"Asia/Tehran", 3262, 1}, // zone 318
  {"Asia/Tel_Aviv", 3107, 151}, // zone 319
  {"Asia/Thimbu", 2742, 1}, // zone 320
  {"Asia/Thimphu", 2742, 1}, // zone 321
  {"Asia/Tokyo", 2744, 1}, // zone 322
  {"Asia/Tomsk", 2133, 1}, // zone 323
  {"Asia/Ujung_Pandang", 2132, 1}, // zone 324
  {"Asia/Ulaanbaatar", 2132, 1}, // zone 325
  {"Asia/Ulan_Bator", 2132, 1}, // zone 326
  {"Asia/Urumqi", 2742, 1}, // zone 327
  {"Asia/Ust-Nera", 2134, 1}, // zone 328
  {"Asia/Vientiane", 2133, 1}, // zone 329
  {"Asia/Vladivostok", 2134, 1}, // zone 330
  {"Asia/Yakutsk", 2744, 1}, // zone 331
  {"Asia/Yangon", 3261, 1}, // zone 332
  {"Asia/Yekaterinburg", 2286, 1}, // zone 333
  {"Asia/Yerevan", 2590, 1}, // zone 334
  {"Atlantic/Azores", 3263, 151}, // zone 335
  {"Atlantic/Bermuda", 1223, 151}, // zone 336
  {"Atlantic/Canary", 3414, 151}, // zone 337
  {"Atlantic/Cape_Verde", 3565, 1}, // zone 338
  {"Atlantic/Faeroe", 3414, 151}, // zone 339
  {"Atlantic/Faroe", 3414, 151}, // zone 340
  {"Atlantic/Jan_Mayen", 159, 151}, // zone 341
  {"Atlantic/Madeira", 3414, 151}, // zone 342
  {"Atlantic/Reykjavik", 0, 1}, // zone 343
  {"Atlantic/South_Georgia", 1825, 1}, // zone 344
  {"Atlantic/St_Helena", 0, 1}, // zone 345
  {"Atlantic/Stanley", 613, 1}, // zone 346
  {"Australia/ACT", 2135, 151}, // zone 347
  {"Australia/Adelaide", 3566, 151}, // zone 348
  {"Australia/Brisbane", 2134, 1}, // zone 349
  {"Australia/Broken_Hill", 3566, 151}, // zone 350
  {"Australia/Canberra", 2135, 151}, // zone 351
  {"Australia/Currie", 2135, 151}, // zone 352
  {"Australia/Darwin", 3717, 1}, // zone 353
  {"Australia/Eucla", 3718, 1}, // zone 354
  {"Australia/Hobart", 2135, 151}, // zone 355
  {"Australia/LHI", 3719, 151}, // zone 356
  {"Australia/Lindeman", 2134, 1}, // zone 357
  {"Australia/Lord_Howe", 3719, 151}, // zone 358
  {"Australia/Melbourne", 2135, 151}, // zone 359
  {"Australia/NSW", 2135, 151}, // zone 360
  {"Australia/North", 3717, 1}, // zone 361
  {"Australia/Perth", 2132, 1}, // zone 362
  {"Australia/Queensland", 2134, 1}, // zone 363
  {"Australia/South", 3566, 151}, // zone 364
  {"Australia/Sydney", 2135, 151}, // zone 365
  {"Australia/Tasmania", 2135, 151}, // zone 366
  {"Australia/Victoria", 2135, 151}, // zone 367
  {"Australia/West", 2132, 1}, // zone 368
  {"Australia/Yancowinna", 3566, 151}, // zone 369
  {"Brazil/Acre", 614, 1}, // zone 370
  {"Brazil/DeNoronha", 1825, 1}, // zone 371
  {"Brazil/East", 613, 1}, // zone 372
  {"Brazil/West", 612, 1}, // zone 373
  {"CET", 159, 151}, // zone 374
  {"CST6CDT", 767, 151}, // zone 375
  {"Canada/Atlantic", 1223, 151}, // zone 376
  {"Canada/Central", 1826, 2}, // zone 377
  {"Canada/Eastern", 919, 151}, // zone 378
  {"Canada/Mountain", 1070, 2}, // zone 379
  {"Canada/Newfoundland", 1979, 151}, // zone 380
  {"Canada/Pacific", 2130, 2}, // zone 381
  {"Canada/Saskatchewan", 615, 1}, // zone 382
  {"Canada/Yukon", 918, 1}, // zone 383
  {"Chile/Continental", 1828, 151}, // zone 384
  {"Chile/EasterIsland", 3870, 151}, // zone 385
  {"Cuba", 1525, 149}, // zone 386
  {"EET", 2745, 151}, // zone 387
  {"EST", 614, 1}, // zone 388
  {"EST5EDT", 919, 151}, // zone 389
  {"Egypt", 4, 151}, // zone 390
  {"Eire", 3414, 151}, // zone 391
  {"Etc/GMT", 0, 1}, // zone 392
  {"Etc/GMT+0", 0, 1}, // zone 393
  {"Etc/GMT+1", 3565, 1}, // zone 394
  {"Etc/GMT+10", 4021, 1}, // zone 395
  {"Etc/GMT+11", 4022, 1}, // zone 396
  {"Etc/GMT+12", 4023, 1}, // zone 397
  {"Etc/GMT+2", 1825, 1}, // zone 398
  {"Etc/GMT+3", 613, 1}, // zone 399
  {"Etc/GMT+4", 612, 1}, // zone 400
  {"Etc/GMT+5", 614, 1}, // zone 401
  {"Etc/GMT+6", 615, 1}, // zone 402
  {"Etc/GMT+7", 918, 1}, // zone 403
  {"Etc/GMT+8", 4024, 1}, // zone 404
  {"Etc/GMT+9", 4025, 1}, // zone 405
  {"Etc/GMT-0", 0, 1}, // zone 406
  {"Etc/GMT-1", 2, 1}, // zone 407
  {"Etc/GMT-10", 2134, 1}, // zone 408
  {"Etc/GMT-11", 3260, 1}, // zone 409
  {"Etc/GMT-12", 2589, 1}, // zone 410
  {"Etc/GMT-13", 4026, 1}, // zone 411
  {"Etc/GMT-14", 4027, 1}, // zone 412
  {"Etc/GMT-2", 3, 1}, // zone 413
  {"Etc/GMT-3", 1, 1}, // zone 414
  {"Etc/GMT-4", 2590, 1}, // zone 415
  {"Etc/GMT-5", 2286, 1}, // zone 416
  {"Etc/GMT-6", 2742, 1}, // zone 417
  {"Etc/GMT-7", 2133, 1}, // zone 418
  {"Etc/GMT-8", 2132, 1}, // zone 419
  {"Etc/GMT-9", 2744, 1}, // zone 420
  {"Etc/GMT0", 0, 1}, // zone 421
  {"Etc/Greenwich", 0, 1}, // zone 422
  {"Etc/UCT", 0, 1}, // zone 423
  {"Etc/UTC", 0, 1}, // zone 424
  {"Etc/Universal", 0, 1}, // zone 425
  {"Etc/Zulu", 0, 1}, // zone 426
  {"Europe/Amsterdam", 159, 151}, // zone 427
  {"Europe/Andorra", 159, 151}, // zone 428
  {"Europe/Astrakhan", 2590, 1}, // zone 429
  {"Europe/Athens", 2745, 151}, // zone 430
  {"Europe/Belfast", 3414, 151}, // zone 431
  {"Europe/Belgrade", 159, 151}, // zone 432
  {"Europe/Berlin", 159, 151}, // zone 433
  {"Europe/Bratislava", 159, 151}, // zone 434
  {"Europe/Brussels", 159, 151}, // zone 435
  {"Europe/Bucharest", 2745, 151}, // zone 436
  {"Europe/Budapest", 159, 151}, // zone 437
  {"Europe/Busingen", 159, 151}, // zone 438
  {"Europe/Chisinau", 2745, 151}, // zone 439
  {"Europe/Copenhagen", 159, 151}, // zone 440
  {"Europe/Dublin", 3414, 151}, // zone 441
  {"Europe/Gibraltar", 159, 151}, // zone 442
  {"Europe/Guernsey", 3414, 151}, // zone 443
  {"Europe/Helsinki", 2745, 151}, // zone 444
  {"Europe/Isle_of_Man", 3414, 151}, // zone 445
  {"Europe/Istanbul", 1, 1}, // zone 446
  {"Europe/Jersey", 3414, 151}, // zone 447
  {"Europe/Kaliningrad", 3, 1}, // zone 448
  {"Europe/Kiev", 2745, 151}, // zone 449
  {"Europe/Kirov", 1, 1}, // zone 450
  {"Europe/Kyiv", 2745, 151}, // zone 451
  {"Europe/Lisbon", 3414, 151}, // zone 452
  {"Europe/Ljubljana", 159, 151}, // zone 453
  {"Europe/London", 3414, 151}, // zone 454
  {"Europe/Luxembourg", 159, 151}, // zone 455
  {"Europe/Madrid", 159, 151}, // zone 456
  {"Europe/Malta", 159, 151}, // zone 457
  {"Europe/Mariehamn", 2745, 151}, // zone 458
  {"Europe/Minsk", 1, 1}, // zone 459
  {"Europe/Monaco", 159, 151}, // zone 460
  {"Europe/Moscow", 1, 1}, // zone 461
  {"Europe/Nicosia", 2745, 151}, // zone 462
  {"Europe/Oslo", 159, 151}, // zone 463
  {"Europe/Paris", 159, 151}, // zone 464
  {"Europe/Podgorica", 159, 151}, // zone 465
  {"Europe/Prague", 159, 151}, // zone 466
  {"Europe/Riga", 2745, 151}, // zone 467
  {"Europe/Rome", 159, 151}, // zone 468
  {"Europe/Samara", 2590, 1}, // zone 469
  {"Europe/San_Marino", 159, 151}, // zone 470
  {"Europe/Sarajevo", 159, 151}, // zone 471
  {"Europe/Saratov", 2590, 1}, // zone 472
  {"Europe/Simferopol", 1, 1}, // zone 473
  {"Europe/Skopje", 159, 151}, // zone 474
  {"Europe/Sofia", 2745, 151}, // zone 475
  {"Europe/Stockholm", 159, 151}, // zone 476
  {"Europe/Tallinn", 2745, 151}, // zone 477
  {"Europe/Tirane", 159, 151}, // zone 478
  {"Europe/Tiraspol", 2745, 151}, // zone 479
  {"Europe/Ulyanovsk", 2590, 1}, // zone 480
  {"Europe/Uzhgorod", 2745, 151}, // zone 481
  {"Europe/Vaduz", 159, 151}, // zone 482
  {"Europe/Vatican", 159, 151}, // zone 483
  {"Europe/Vienna", 159, 151}, // zone 484
  {"Europe/Vilnius", 2745, 151}, // zone 485
  {"Europe/Volgograd", 1, 1}, // zone 486
  {"Europe/Warsaw", 159, 151}, // zone 487
  {"Europe/Zagreb", 159, 151}, // zone 488
  {"Europe/Zaporozhye", 2745, 151}, // zone 489
  {"Europe/Zurich", 159, 151}, // zone 490
  {"GB", 3414, 151}, // zone 491
  {"GB-Eire", 3414, 151}, // zone 492
  {"GMT", 0, 1}, // zone 493
  {"GMT+0", 0, 1}, // zone 494
  {"GMT-0", 0, 1}, // zone 495
  {"GMT0", 0, 1}, // zone 496
  {"Greenwich", 0, 1}, // zone 497
  {"HST", 4021, 1}, // zone 498
  {"Hongkong", 2132, 1}, // zone 499
  {"Iceland", 0, 1}, // zone 500
  {"Indian/Antananarivo", 1, 1}, // zone 501
  {"Indian/Chagos", 2742, 1}, // zone 502
  {"Indian/Christmas", 2133, 1}, // zone 503
  {"Indian/Cocos", 3261, 1}, // zone 504
  {"Indian/Comoro", 1, 1}, // zone 505
  {"Indian/Kerguelen", 2286, 1}, // zone 506
  {"Indian/Mahe", 2590, 1}, // zone 507
  {"Indian/Maldives", 2286, 1}, // zone 508
  {"Indian/Mauritius", 2590, 1}, // zone 509
  {"Indian/Mayotte", 1, 1}, // zone 510
  {"Indian/Reunion", 2590, 1}, // zone 511
  {"Iran", 3262, 1}, // zone 512
  {"Israel", 3107, 151}, // zone 513
  {"Jamaica", 614, 1}, // zone 514
  {"Japan", 2744, 1}, // zone 515
  {"Kwajalein", 2589, 1}, // zone 516
  {"Libya", 3, 1}, // zone 517
  {"MET", 159, 151}, // zone 518
  {"MST", 918, 1}, // zone 519
  {"MST7MDT", 616, 151}, // zone 520
  {"Mexico/BajaNorte", 1072, 151}, // zone 521
  {"Mexico/BajaSur", 918, 1}, // zone 522
  {"Mexico/General", 615, 1}, // zone 523
  {"NZ", 2287, 151}, // zone 524
  {"NZ-CHAT", 4028, 151}, // zone 525
  {"Navajo", 616, 151}, // zone 526
  {"PRC", 2132, 1}, // zone 527
  {"PST8PDT", 1072, 151}, // zone 528
  {"Pacific/Apia", 4026, 1}, // zone 529
  {"Pacific/Auckland", 2287, 151}, // zone 530
  {"Pacific/Bougainville", 3260, 1}, // zone 531
  {"Pacific/Chatham", 4028, 151}, // zone 532
  {"Pacific/Chuuk", 2134, 1}, // zone 533
  {"Pacific/Easter", 3870, 151}, // zone 534
  {"Pacific/Efate", 3260, 1}, // zone 535
  {"Pacific/Enderbury", 4026, 1}, // zone 536
  {"Pacific/Fakaofo", 4026, 1}, // zone 537
  {"Pacific/Fiji", 2589, 1}, // zone 538
  {"Pacific/Funafuti", 2589, 1}, // zone 539
  {"Pacific/Galapagos", 615, 1}, // zone 540
  {"Pacific/Gambier", 4025, 1}, // zone 541
  {"Pacific/Guadalcanal", 3260, 1}, // zone 542
  {"Pacific/Guam", 2134, 1}, // zone 543
  {"Pacific/Honolulu", 4021, 1}, // zone 544
  {"Pacific/Johnston", 4021, 1}, // zone 545
  {"Pacific/Kanton", 4026, 1}, // zone 546
  {"Pacific/Kiritimati", 4027, 1}, // zone 547
  {"Pacific/Kosrae", 3260, 1}, // zone 548
  {"Pacific/Kwajalein", 2589, 1}, // zone 549
  {"Pacific/Majuro", 2589, 1}, // zone 550
  {"Pacific/Marquesas", 4179, 1}, // zone 551
  {"Pacific/Midway", 4022, 1}, // zone 552
  {"Pacific/Nauru", 2589, 1}, // zone 553
  {"Pacific/Niue", 4022, 1}, // zone 554
  {"Pacific/Norfolk", 4180, 151}, // zone 555
  {"Pacific/Noumea", 3260, 1}, // zone 556
  {"Pacific/Pago_Pago", 4022, 1}, // zone 557
  {"Pacific/Palau", 2744, 1}, // zone 558
  {"Pacific/Pitcairn", 4024, 1}, // zone 559
  {"Pacific/Pohnpei", 3260, 1}, // zone 560
  {"Pacific/Ponape", 3260, 1}, // zone 561
  {"Pacific/Port_Moresby", 2134, 1}, // zone 562
  {"Pacific/Rarotonga", 4021, 1}, // zone 563
  {"Pacific/Saipan", 2134, 1}, // zone 564
  {"Pacific/Samoa", 4022, 1}, // zone 565
  {"Pacific/Tahiti", 4021, 1}, // zone 566
  {"Pacific/Tarawa", 2589, 1}, // zone 567
  {"Pacific/Tongatapu", 4026, 1}, // zone 568
  {"Pacific/Truk", 2134, 1}, // zone 569
  {"Pacific/Wake", 2589, 1}, // zone 570
  {"Pacific/Wallis", 2589, 1}, // zone 571
  {"Pacific/Yap", 2134, 1}, // zone 572
  {"Poland", 159, 151}, // zone 573
  {"Portugal", 3414, 151}, // zone 574
  {"ROC", 2132, 1}, // zone 575
  {"ROK", 2744, 1}, // zone 576
  {"Singapore", 2132, 1}, // zone 577
  {"Turkey", 1, 1}, // zone 578
  {"UCT", 0, 1}, // zone 579
  {"US/Alaska", 461, 151}, // zone 580
  {"US/Aleutian", 310, 151}, // zone 581
  {"US/Arizona", 918, 1}, // zone 582
  {"US/Central", 767, 151}, // zone 583
  {"US/East-Indiana", 919, 151}, // zone 584
  {"US/Eastern", 919, 151}, // zone 585
  {"US/Hawaii", 4021, 1}, // zone 586
  {"US/Indiana-Starke", 767, 151}, // zone 587
  {"US/Michigan", 919, 151}, // zone 588
  {"US/Mountain", 616, 151}, // zone 589
  {"US/Pacific", 1072, 151}, // zone 590
  {"US/Samoa", 4022, 1}, // zone 591
  {"UTC", 0, 1}, // zone 592
  {"Universal", 0, 1}, // zone 593
  {"W-SU", 1, 1}, // zone 594
  {"WET", 3414, 151}, // zone 595
  {"Zulu", 0, 1}, // zone 596
};
const size_t HUNTER_TIMEZONE_COUNT = sizeof(HUNTER_TIMEZONES) / sizeof(HUNTER_TIMEZONES[0]);
const char HUNTER_TIMEZONE_NAMES_JSON[] = R"zones(["Africa/Abidjan","Africa/Accra","Africa/Addis_Ababa","Africa/Algiers","Africa/Asmara","Africa/Asmera","Africa/Bamako","Africa/Bangui","Africa/Banjul","Africa/Bissau","Africa/Blantyre","Africa/Brazzaville","Africa/Bujumbura","Africa/Cairo","Africa/Casablanca","Africa/Ceuta","Africa/Conakry","Africa/Dakar","Africa/Dar_es_Salaam","Africa/Djibouti","Africa/Douala","Africa/El_Aaiun","Africa/Freetown","Africa/Gaborone","Africa/Harare","Africa/Johannesburg","Africa/Juba","Africa/Kampala","Africa/Khartoum","Africa/Kigali","Africa/Kinshasa","Africa/Lagos","Africa/Libreville","Africa/Lome","Africa/Luanda","Africa/Lubumbashi","Africa/Lusaka","Africa/Malabo","Africa/Maputo","Africa/Maseru","Africa/Mbabane","Africa/Mogadishu","Africa/Monrovia","Africa/Nairobi","Africa/Ndjamena","Africa/Niamey","Africa/Nouakchott","Africa/Ouagadougou","Africa/Porto-Novo","Africa/Sao_Tome","Africa/Timbuktu","Africa/Tripoli","Africa/Tunis","Africa/Windhoek","America/Adak","America/Anchorage","America/Anguilla","America/Antigua","America/Araguaina","America/Argentina/Buenos_Aires","America/Argentina/Catamarca","America/Argentina/ComodRivadavia","America/Argentina/Cordoba","America/Argentina/Jujuy","America/Argentina/La_Rioja","America/Argentina/Mendoza","America/Argentina/Rio_Gallegos","America/Argentina/Salta","America/Argentina/San_Juan","America/Argentina/San_Luis","America/Argentina/Tucuman","America/Argentina/Ushuaia","America/Aruba","America/Asuncion","America/Atikokan","America/Atka","America/Bahia","America/Bahia_Banderas","America/Barbados","America/Belem","America/Belize","America/Blanc-Sablon","America/Boa_Vista","America/Bogota","America/Boise","America/Buenos_Aires","America/Cambridge_Bay","America/Campo_Grande","America/Cancun","America/Caracas","America/Catamarca","America/Cayenne","America/Cayman","America/Chicago","America/Chihuahua","America/Ciudad_Juarez","America/Coral_Harbour","America/Cordoba","America/Costa_Rica","America/Coyhaique","America/Creston","America/Cuiaba","America/Curacao","America/Danmarkshavn","America/Dawson","America/Dawson_Creek","America/Denver","America/Detroit","America/Dominica","America/Edmonton","America/Eirunepe","America/El_Salvador","America/Ensenada","America/Fort_Nelson","America/Fort_Wayne","America/Fortaleza","America/Glace_Bay","America/Godthab","America/Goose_Bay","America/Grand_Turk","America/Grenada","America/Guadeloupe","America/Guatemala","America/Guayaquil","America/Guyana","America/Halifax","America/Havana","America/Hermosillo","America/Indiana/Indianapolis","America/Indiana/Knox","America/Indiana/Marengo","America/Indiana/Petersburg","America/Indiana/Tell_City","America/Indiana/Vevay","America/Indiana/Vincennes","America/Indiana/Winamac","America/Indianapolis","America/Inuvik","America/Iqaluit","America/Jamaica","America/Jujuy","America/Juneau","America/Kentucky/Louisville","America/Kentucky/Monticello","America/Knox_IN","America/Kralendijk","America/La_Paz","America/Lima","America/Los_Angeles","America/Louisville","America/Lower_Princes","America/Maceio","America/Managua","America/Manaus","America/Marigot","America/Martinique","America/Matamoros","America/Mazatlan","America/Mendoza","America/Menominee","America/Merida","America/Metlakatla","America/Mexico_City","America/Miquelon","America/Moncton","America/Monterrey","America/Montevideo","America/Montreal","America/Montserrat","America/Nassau","America/New_York","America/Nipigon","America/Nome","America/Noronha","America/North_Dakota/Beulah","America/North_Dakota/Center","America/North_Dakota/New_Salem","America/Nuuk","America/Ojinaga","America/Panama","America/Pangnirtung","America/Paramaribo","America/Phoenix","America/Port-au-Prince","America/Port_of_Spain","America/Porto_Acre","America/Porto_Velho","America/Puerto_Rico","America/Punta_Arenas","America/Rainy_River","America/Rankin_Inlet","America/Recife","America/Regina","America/Resolute","America/Rio_Branco","America/Rosario","America/Santa_Isabel","America/Santarem","America/Santiago","America/Santo_Domingo","America/Sao_Paulo","America/Scoresbysund","America/Shiprock","America/Sitka","America/St_Barthelemy","America/St_Johns","America/St_Kitts","America/St_Lucia","America/St_Thomas","America/St_Vincent","America/Swift_Current","America/Tegucigalpa","America/Thule","America/Thunder_Bay","America/Tijuana","America/Toronto","America/Tortola","America/Vancouver","America/Virgin","America/Whitehorse","America/Winnipeg","America/Yakutat","America/Yellowknife","Antarctica/Casey","Antarctica/Davis","Antarctica/DumontDUrville","Antarctica/Macquarie","Antarctica/Mawson","Antarctica/McMurdo","Antarctica/Palmer","Antarctica/Rothera","Antarctica/South_Pole","Antarctica/Syowa","Antarctica/Troll","Antarctica/Vostok","Arctic/Longyearbyen","Asia/Aden","Asia/Almaty","Asia/Amman","Asia/Anadyr","Asia/Aqtau","Asia/Aqtobe","Asia/Ashgabat","Asia/Ashkhabad","Asia/Atyrau","Asia/Baghdad","Asia/Bahrain","Asia/Baku","Asia/Bangkok","Asia/Barnaul","Asia/Beirut","Asia/Bishkek","Asia/Brunei","Asia/Calcutta","Asia/Chita","Asia/Choibalsan","Asia/Chongqing","Asia/Chungking","Asia/Colombo","Asia/Dacca","Asia/Damascus","Asia/Dhaka","Asia/Dili","Asia/Dubai","Asia/Dushanbe","Asia/Famagusta","Asia/Gaza","Asia/Harbin","Asia/Hebron","Asia/Ho_Chi_Minh","Asia/Hong_Kong","Asia/Hovd","Asia/Irkutsk","Asia/Istanbul","Asia/Jakarta","Asia/Jayapura","Asia/Jerusalem","Asia/Kabul","Asia/Kamchatka","Asia/Karachi","Asia/Kashgar","Asia/Kathmandu","Asia/Katmandu","Asia/Khandyga","Asia/Kolkata","Asia/Krasnoyarsk","Asia/Kuala_Lumpur","Asia/Kuching","Asia/Kuwait","Asia/Macao","Asia/Macau","Asia/Magadan","Asia/Makassar","Asia/Manila","Asia/Muscat","Asia/Nicosia","Asia/Novokuznetsk","Asia/Novosibirsk","Asia/Omsk","Asia/Oral","Asia/Phnom_Penh","Asia/Pontianak","Asia/Pyongyang","Asia/Qatar","Asia/Qostanay","Asia/Qyzylorda","Asia/Rangoon","Asia/Riyadh","Asia/Saigon","Asia/Sakhalin","Asia/Samarkand","Asia/Seoul","Asia/Shanghai","Asia/Singapore","Asia/Srednekolymsk","Asia/Taipei","Asia/Tashkent","Asia/Tbilisi","Asia/Tehran","Asia/Tel_Aviv","Asia/Thimbu","Asia/Thimphu","Asia/Tokyo","Asia/Tomsk","Asia/Ujung_Pandang","Asia/Ulaanbaatar","Asia/Ulan_Bator","Asia/Urumqi","Asia/Ust-Nera","Asia/Vientiane","Asia/Vladivostok","Asia/Yakutsk","Asia/Yangon","Asia/Yekaterinburg","Asia/Yerevan","Atlantic/Azores","Atlantic/Bermuda","Atlantic/Canary","Atlantic/Cape_Verde","Atlantic/Faeroe","Atlantic/Faroe","Atlantic/Jan_Mayen","Atlantic/Madeira","Atlantic/Reykjavik","Atlantic/South_Georgia","Atlantic/St_Helena","Atlantic/Stanley","Australia/ACT","Australia/Adelaide","Australia/Brisbane","Australia/Broken_Hill","Australia/Canberra","Australia/Currie","Australia/Darwin","Australia/Eucla","Australia/Hobart","Australia/LHI","Australia/Lindeman","Australia/Lord_Howe","Australia/Melbourne","Australia/NSW","Australia/North","Australia/Perth","Australia/Queensland","Australia/South","Australia/Sydney","Australia/Tasmania","Australia/Victoria","Australia/West","Australia/Yancowinna","Brazil/Acre","Brazil/DeNoronha","Brazil/East","Brazil/West","CET","CST6CDT","Canada/Atlantic","Canada/Central","Canada/Eastern","Canada/Mountain","Canada/Newfoundland","Canada/Pacific","Canada/Saskatchewan","Canada/Yukon","Chile/Continental","Chile/EasterIsland","Cuba","EET","EST","EST5EDT","Egypt","Eire","Etc/GMT","Etc/GMT+0","Etc/GMT+1","Etc/GMT+10","Etc/GMT+11","Etc/GMT+12","Etc/GMT+2","Etc/GMT+3","Etc/GMT+4","Etc/GMT+5","Etc/GMT+6","Etc/GMT+7","Etc/GMT+8","Etc/GMT+9","Etc/GMT-0","Etc/GMT-1","Etc/GMT-10","Etc/GMT-11","Etc/GMT-12","Etc/GMT-13","Etc/GMT-14","Etc/GMT-2","Etc/GMT-3","Etc/GMT-4","Etc/GMT-5","Etc/GMT-6","Etc/GMT-7","Etc/GMT-8","Etc/GMT-9","Etc/GMT0","Etc/Greenwich","Etc/UCT","Etc/UTC","Etc/Universal","Etc/Zulu","Europe/Amsterdam","Europe/Andorra","Europe/Astrakhan","Europe/Athens","Europe/Belfast","Europe/Belgrade","Europe/Berlin","Europe/Bratislava","Europe/Brussels","Europe/Bucharest","Europe/Budapest","Europe/Busingen","Europe/Chisinau","Europe/Copenhagen","Europe/Dublin","Europe/Gibraltar","Europe/Guernsey","Europe/Helsinki","Europe/Isle_of_Man","Europe/Istanbul","Europe/Jersey","Europe/Kaliningrad","Europe/Kiev","Europe/Kirov","Europe/Kyiv","Europe/Lisbon","Europe/Ljubljana","Europe/London","Europe/Luxembourg","Europe/Madrid","Europe/Malta","Europe/Mariehamn","Europe/Minsk","Europe/Monaco","Europe/Moscow","Europe/Nicosia","Europe/Oslo","Europe/Paris","Europe/Podgorica","Europe/Prague","Europe/Riga","Europe/Rome","Europe/Samara","Europe/San_Marino","Europe/Sarajevo","Europe/Saratov","Europe/Simferopol","Europe/Skopje","Europe/Sofia","Europe/Stockholm","Europe/Tallinn","Europe/Tirane","Europe/Tiraspol","Europe/Ulyanovsk","Europe/Uzhgorod","Europe/Vaduz","Europe/Vatican","Europe/Vienna","Europe/Vilnius","Europe/Volgograd","Europe/Warsaw","Europe/Zagreb","Europe/Zaporozhye","Europe/Zurich","GB","GB-Eire","GMT","GMT+0","GMT-0","GMT0","Greenwich","HST","Hongkong","Iceland","Indian/Antananarivo","Indian/Chagos","Indian/Christmas","Indian/Cocos","Indian/Comoro","Indian/Kerguelen","Indian/Mahe","Indian/Maldives","Indian/Mauritius","Indian/Mayotte","Indian/Reunion","Iran","Israel","Jamaica","Japan","Kwajalein","Libya","MET","MST","MST7MDT","Mexico/BajaNorte","Mexico/BajaSur","Mexico/General","NZ","NZ-CHAT","Navajo","PRC","PST8PDT","Pacific/Apia","Pacific/Auckland","Pacific/Bougainville","Pacific/Chatham","Pacific/Chuuk","Pacific/Easter","Pacific/Efate","Pacific/Enderbury","Pacific/Fakaofo","Pacific/Fiji","Pacific/Funafuti","Pacific/Galapagos","Pacific/Gambier","Pacific/Guadalcanal","Pacific/Guam","Pacific/Honolulu","Pacific/Johnston","Pacific/Kanton","Pacific/Kiritimati","Pacific/Kosrae","Pacific/Kwajalein","Pacific/Majuro","Pacific/Marquesas","Pacific/Midway","Pacific/Nauru","Pacific/Niue","Pacific/Norfolk","Pacific/Noumea","Pacific/Pago_Pago","Pacific/Palau","Pacific/Pitcairn","Pacific/Pohnpei","Pacific/Ponape","Pacific/Port_Moresby","Pacific/Rarotonga","Pacific/Saipan","Pacific/Samoa","Pacific/Tahiti","Pacific/Tarawa","Pacific/Tongatapu","Pacific/Truk","Pacific/Wake","Pacific/Wallis","Pacific/Yap","Poland","Portugal","ROC","ROK","Singapore","Turkey","UCT","US/Alaska","US/Aleutian","US/Arizona","US/Central","US/East-Indiana","US/Eastern","US/Hawaii","US/Indiana-Starke","US/Michigan","US/Mountain","US/Pacific","US/Samoa","UTC","Universal","W-SU","WET","Zulu"])zones";
int hunterTimezoneIndex(const char* name) {
  int low = 0, high = HUNTER_TIMEZONE_COUNT - 1;
  while (low <= high) {
    int mid = (low + high) / 2;
    int order = strcmp(name, HUNTER_TIMEZONES[mid].name);
    if (order == 0) return mid;
    if (order < 0) high = mid - 1; else low = mid + 1;
  }
  return -1;
}
int32_t hunterTimezoneOffset(int zone, int64_t utc) {
  const HunterTimezone& row = HUNTER_TIMEZONES[zone];
  int low = row.first, high = row.first + row.count;
  while (low + 1 < high) {
    int mid = (low + high) / 2;
    if (HUNTER_ZONE_TRANSITIONS[mid].utc <= utc) low = mid; else high = mid;
  }
  return HUNTER_ZONE_TRANSITIONS[low].offset;
}
