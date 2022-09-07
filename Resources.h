#ifndef IPP_Resources_Header
#define IPP_Resources_Header

#ifndef FUSION_RUNTIME_ONLY

#define RIPP_NAME 101
#define RIPP_AUTHOR 102
#define RIPP_COPYRIGHT 103
#define RIPP_DESCRIPTION 104
#define RIPP_WEBSITE 105
#define RIPP_HELP 106

#define RIPPAM_SET_CURRENT_GROUP 201
#define RIPPAM_CURRENT_GROUP 202
#define RIPPAM_SET_VALUE 203
#define RIPPAM_SET_STRING 204
#define RIPPAM_SET_STRING_MD5 205
#define RIPPAM_SAVE_OBJECT 206
#define RIPPAM_LOAD_OBJECT 207
#define RIPPAM_SAVE_POSITION 208
#define RIPPAM_SAVE_GLOBALS 209
#define RIPPAM_LOAD_GLOBALS 210
#define RIPPAM_DELETE_GROUP 211
#define RIPPAM_DELETE_ITEM 212
#define RIPPAM_RENAME_GROUP 213
#define RIPPAM_RENAME_ITEM 214
#define RIPPAM_MOVE_ITEM_ANOTHER 215
#define RIPPAM_SETTING_ITEMS 216
#define RIPPAM_MOVING_COPYING 217
#define RIPPAM_MOVE_ITEM 218
#define RIPPAM_COPY_GROUP 219
#define RIPPAM_COPY_ITEM 220
#define RIPPAM_DELETING 221
#define RIPPAM_DELETE_ITEM_EVERYWHERE 222
#define RIPPAM_DELETE_PATTERN 223
#define RIPPAM_CLEAR_ALL 224
#define RIPPAM_SEARCHING 225
#define RIPPAM_PERFORM_SEARCH 226
#define RIPPAM_CLEAR_RESULTS 227
#define RIPPAM_SEARCH_STORE_INI 228
#define RIPPAM_SEARCH_STORE_INIPP 229
#define RIPPAM_FIND_SUBGROUPS 230
#define RIPPAM_FIND_REPEATS_ITEM 231
#define RIPPAM_MERGING 232
#define RIPPAM_MERGE_WITH_FILE 233
#define RIPPAM_MERGE_GROUP_WITH_FILE 234
#define RIPPAM_MERGE_WITH_INIPP 235
#define RIPPAM_MERGE_GROUP_WITH_INIPP 236
#define RIPPAM_LOADING_SAVING 237
#define RIPPAM_CHANGE_FILE 238
#define RIPPAM_SAVE_FILE 239
#define RIPPAM_CLOSE 240
#define RIPPAM_LOAD_FROM_STRING 241
#define RIPPAM_CHANGE_GLOBAL_KEY 242
#define RIPPAM_SETTINGS 243
#define RIPPAM_SET_READ_ONLY 244
#define RIPPAM_SET_ENCRYPTION 245
#define RIPPAM_SET_COMPRESSION 246
#define RIPPAM_SET_AUTO_LOAD_SAVE 247
#define RIPPAM_EXPORT 248
#define RIPPAM_SAVE_AS 249
#define RIPPAM_BACKUP_TO 250
#define RIPPAM_IO_CSV 251
#define RIPPAM_IO_CSV_ARRAY 252
#define RIPPAM_REORDER_GROUP 253
#define RIPPAM_ADVANCED_FEATURES 254
#define RIPPAM_ADD_REPEATED_ITEM 255
#define RIPPAM_SET_PART_IN_ITEM 256
#define RIPPAM_UNDO 257
#define RIPPAM_PERFORM_UNDO 258
#define RIPPAM_PERFORM_REDO 259
#define RIPPAM_CLEAR_STACKS 260
#define RIPPAM_MANUAL_MODE 261
#define RIPPAM_SET_MODE 262
#define RIPPAM_FORCE_ADD_UNDO_BLOCK 263
#define RIPPAM_DIALOG_BOX 264
#define RIPPAM_OPEN_DIALOG 265
#define RIPPAM_CLOSE_DIALOG 266
#define RIPPAM_REFRESH_DIALOG 267
#define RIPPAM_CHART_OBJECT 268
#define RIPPAM_PUT_DATA_INTO_CHART 269
#define RIPPAM_GET_DATA_FROM_CHART 270
#define RIPPAM_PUT_SETTINGS_INTO_CHART 271
#define RIPPAM_GET_SETTINGS_FROM_CHART 272
#define RIPPAM_ARRAY 273
#define RIPPAM_GET_DATA_FROM_ARRAY 274
#define RIPPAM_PUT_DATA_INTO_ARRAY 275
#define RIPPAM_ADVANCED_SETTINGS 276
#define RIPPAM_SET_CASE_SENSITIVE 277
#define RIPPAM_SET_ESCAPE_CHARS 278
#define RIPPAM_SET_ALWAYS_QUOTE 279
#define RIPPAM_SET_REPEAT_MODES 280
#define RIPPAM_SET_NEWLINE_STR 281
#define RIPPAM_SET_DEFAULT_DIR 282
#define RIPPAM_SET_GROUP_OPTS 283
#define RIPPAM_EXTRA_FUNCTIONS 284
#define RIPPAM_COMPRESS_FILE 285
#define RIPPAM_DECOMPRESS_FILE 286

#define RIPPCM_GROUP_EXISTS 301
#define RIPPCM_GROUP_HAS_ITEM 302
#define RIPPCM_ITEM_HAS_HASH 303
#define RIPPCM_ON_OPEN 304
#define RIPPCM_ON_CLOSE 305
#define RIPPCM_ON_MODIFIED 306
#define RIPPCM_SEARCH_RESULTS 307
#define RIPPCM_ON_RESULTS_FOUND 308
#define RIPPCM_ON_NO_RESULTS_FOUND 309
#define RIPPCM_HAS_UNDO 310
#define RIPPCM_HAS_REDO 311
#define RIPPCM_ON_MERGE 312
#define RIPPCM_ON_DATA_MODIFIED 313
#define RIPPCM_ON_NEW_ITEM 314
#define RIPPCM_ON_SAVE 315
#define RIPPCM_IS_WILDCAT_MATCH 316
#define RIPPCM_ON_UNCOMPRESSION_OK 317
#define RIPPCM_ON_UNCOMPRESSION_FAIL 318

#define RIPPEM_GROUP_GET_ITEM_VALUE 401
#define RIPPEM_GROUP_GET_ITEM_STRING 402
#define RIPPEM_GROUP_GET_POSITION_X 403
#define RIPPEM_GROUP_GET_POSITION_Y 404
#define RIPPEM_GROUP_GET_NTH_ITEM_NAME 405
#define RIPPEM_GROUP_GET_NTH_ITEM_VALUE 406
#define RIPPEM_GROUP_GET_NTH_ITEM_STRING 407
#define RIPPEM_GROUP_GET_NAME 408
#define RIPPEM_GROUP_GET_NUM_ITEMS 409
#define RIPPEM_GROUP_GET_STRINGIFIED 410
#define RIPPEM_GET_ITEM_VALUE 411
#define RIPPEM_GET_ITEM_STRING 412
#define RIPPEM_GET_PART_IN_ITEM 413
#define RIPPEM_GET_POSITION_X 414
#define RIPPEM_GET_POSITION_Y 415
#define RIPPEM_GET_FILENAME 416
#define RIPPEM_GET_STRINGIFIED 417
#define RIPPEM_GET_CSVIFIED 418
#define RIPPEM_COUNTING_CALCS 419
#define RIPPEM_GET_NUM_GROUPS 420
#define RIPPEM_GET_NUM_ITEMS 421
#define RIPPEM_GET_NUM_ITEMS_EVERYWHERE 422
#define RIPPEM_GET_NUM_UNIQUE_ITEM_NAMES 423
#define RIPPEM_PERFORM_CALCULATIONS 424
#define RIPPEM_FIND_MIN_OR_MAX_GROUP 425
#define RIPPEM_LIST_GROUPS_ITEMS 426
#define RIPPEM_GET_NTH_GROUP 427
#define RIPPEM_GET_NTH_ITEM_NAME 428
#define RIPPEM_GET_NTH_ITEM_STRING 429
#define RIPPEM_GET_NTH_ITEM_VALUE 430
#define RIPPEM_GET_SORTED_NTH_ITEM_NAME 431
#define RIPPEM_GET_SORTED_NTH_ITEM_VALUE 432
#define RIPPEM_GET_NTH_ITEM_EVERYWHERE 433
#define RIPPEM_GET_NUM_SEARCH_RESULTS 434
#define RIPPEM_GET_NTH_SEARCH_RESULT_GROUP 435
#define RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_NAME 436
#define RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_VALUE 437
#define RIPPEM_GET_NTH_SEARCH_RESULT_ITEM_STRING 438
#define RIPPEM_GET_NTH_SEARCH_RESULT_PATH 439
#define RIPPEM_HASH_STRING 440
#define RIPPEM_ESCAPE_STRING 441
#define RIPPEM_UNESCAPE_STRING 442

#define RIPPP_GROUP 501
#define RIPPP_CREATE_DUPLICATE_GROUP 502
#define RIPPP_ITEM 503
#define RIPPP_TYPE 504
#define RIPPP_VALUE 505
#define RIPPP_STRING 506
#define RIPPP_SAVE_OBJECT 507
#define RIPPP_SAVE_POSITION 508
#define RIPPP_SAVE_ALT_VALUES 509
#define RIPPP_LOAD_OBJECT 510
#define RIPPP_POSITION 511
#define RIPPP_NEW_NAME 512
#define RIPPP_OVERWRITE_MODE_012 513
#define RIPPP_OLD_NAME 514
#define RIPPP_ALLOW_OVERWRITE 515
#define RIPPP_NEW_GROUP_NAME 516
#define RIPPP_OLD_GROUP_NAME 517
#define RIPPP_OLD_ITEM_NAME 518
#define RIPPP_NEW_ITEM_NAME 519
#define RIPPP_SRC_GROUP_NAME 520
#define RIPPP_DST_GROUP_NAME 521
#define RIPPP_OVERWRITE_MODE_01 522
#define RIPPP_SRC_ITEM 523
#define RIPPP_DST_ITEM 524
#define RIPPP_GROUP_TO_DELETE 525
#define RIPPP_PATTERN_GROUP 526
#define RIPPP_PATTERN_ITEM 527
#define RIPPP_PATTERN_VALUE 528
#define RIPPP_CASE_SENSITIVE 529
#define RIPPP_CONDITION_GROUP 530
#define RIPPP_CONDITION_ITEM 531
#define RIPPP_CONDITION_VALUE 532
#define RIPPP_SEARCH_SETTINGS 533
#define RIPPP_DESTINATION_FILE 534
#define RIPPP_OBJECT_SELECTOR 535
#define RIPPP_FILE_TO_MERGE_FROM 536
#define RIPPP_OTHER_INIPP 537
#define RIPPP_FILE_OR_BLANK 538
#define RIPPP_CLEAR_EXISTING 539
#define RIPPP_FILE_TO_LOAD 540
#define RIPPP_READ_ONLY 541
#define RIPPP_FILE_TO_BACKUP_TO 542
#define RIPPP_COMPRESSED 543
#define RIPPP_ENCRYPTED 544
#define RIPPP_INI_TEXT 545
#define RIPPP_MODE_REPLACE_MERGE 546
#define RIPPP_AUTO_SAVE 547
#define RIPPP_AUTO_LOAD 548
#define RIPPP_USE_IN_GROUPS 549
#define RIPPP_USE_IN_ITEMS 550
#define RIPPP_USE_IN_VALUES 551
#define RIPPP_QUOTE_STRINGS 552
#define RIPPP_REPEAT_MODE_GROUPS 553
#define RIPPP_REPEAT_MODE_ITEMS 554
#define RIPPP_SAVE_REPEATS_AS_ORIGINAL 555
#define RIPPP_NEWLINE_OR_EMPTY 556
#define RIPPP_DEFAULT_DIRECTORY 557
#define RIPPP_FILE_SRC 558
#define RIPPP_FILE_DST 559
#define RIPPP_DIALOG_TITLE 560
#define RIPPP_DIALOG_SETTINGS 561
#define RIPPP_STRING_OR_VALUE 562
#define RIPPP_REPEATED_MODE 563
#define RIPPP_DIALOG_REFRESH_MODE 564
#define RIPPP_IMPORT_EXPORT_MODE 565
#define RIPPP_CSV_FILE 566
#define RIPPP_IMPORT_EXPORT_GROUP 567
#define RIPPP_SEPARATOR_SYMBOL 568
#define RIPPP_CHART_OBJECT 569
#define RIPPP_SUBGROUPS_GROUP 570
#define RIPPP_DEPTH 571
#define RIPPP_ALLOW_SUBGROUPS 572
#define RIPPP_ALLOW_EMPTY_GROUPS 573
#define RIPPP_DATA_SLOT 574
#define RIPPP_SSS_SETTINGS 575
#define RIPPP_PART 576
#define RIPPP_ARRAY_OBJECT 577
#define RIPPP_FILE_SETTINGS 578
#define RIPPP_UNDO_REDO_BOTH 579
#define RIPPP_AUTOMATIC_MODE 580
#define RIPPP_DEFAULT_STRING 581
#define RIPPP_COMPARE_STRING 582
#define RIPPP_PATTERN 583
#define RIPPP_ITEM_S 584
#define RIPPP_DEFAULT_VALUE 585
#define RIPPP_DEFAULT_VALUE_S 586
#define RIPPP_N 587
#define RIPPP_GROUP_S 588
#define RIPPP_SEPARATOR_S 589
#define RIPPP_STRING_S 590
#define RIPPP_KEY_S 591
#define RIPPP_CONSULT_DOCUMENTATION 592

#define RIPPA_SET_CURRENT_GROUP 601
#define RIPPA_SET_VALUE 602
#define RIPPA_SET_STRING 603
#define RIPPA_SET_STRING_MD5 604
#define RIPPA_SAVE_OBJECT 605
#define RIPPA_LOAD_OBJECT 606
#define RIPPA_SAVE_POSITION 607
#define RIPPA_SAVE_GLOBALS 608
#define RIPPA_LOAD_GLOBALS 609
#define RIPPA_DELETE_CURRENT_GROUP 610
#define RIPPA_DELETE_ITEM 611
#define RIPPA_RENAME_CURRENT_GROUP 612
#define RIPPA_RENAME_ITEM 613
#define RIPPA_MOVE_ITEM_ANOTHER 614
#define RIPPA_GROUP_SET_VALUE 615
#define RIPPA_GROUP_SET_STRING 616
#define RIPPA_GROUP_SET_STRING_MD5 617
#define RIPPA_GROUP_SAVE_OBJECT 618
#define RIPPA_GROUP_LOAD_OBJECT 619
#define RIPPA_GROUP_SAVE_POSITION 620
#define RIPPA_GROUP_SAVE_GLOBALS 621
#define RIPPA_GROUP_LOAD_GLOBALS 622
#define RIPPA_RENAME_GROUP 623
#define RIPPA_GROUP_RENAME_ITEM 624
#define RIPPA_MOVE_ITEM 625
#define RIPPA_GROUP_MOVE_ITEM_ANOTHER 626
#define RIPPA_COPY_GROUP 627
#define RIPPA_COPY_ITEM 628
#define RIPPA_DELETE_GROUP 629
#define RIPPA_GROUP_DELETE_ITEM 630
#define RIPPA_DELETE_ITEM_EVERYWHERE 631
#define RIPPA_DELETE_PATTERN 632
#define RIPPA_CLEAR_ALL 633
#define RIPPA_PERFORM_SEARCH 634
#define RIPPA_FIND_REPEATS_ITEM 635
#define RIPPA_CLEAR_RESULTS 636
#define RIPPA_SEARCH_STORE_INI 637
#define RIPPA_SEARCH_STORE_INIPP 638
#define RIPPA_MERGE_WITH_FILE 639
#define RIPPA_MERGE_GROUP_WITH_FILE 640
#define RIPPA_MERGE_WITH_INIPP 641
#define RIPPA_MERGE_GROUP_WITH_INIPP 642
#define RIPPA_NEW_FILE 643
#define RIPPA_LOAD_FILE 644
#define RIPPA_SAVE_FILE 645
#define RIPPA_SAVE_AS 646
#define RIPPA_BACKUP_TO 647
#define RIPPA_CLOSE 648
#define RIPPA_LOAD_FROM_STRING 649
#define RIPPA_SET_AUTO_LOAD_SAVE 650
#define RIPPA_SET_COMPRESSION 651
#define RIPPA_SET_ENCRYPTION 652
#define RIPPA_SET_READ_ONLY 653
#define RIPPA_SET_CASE_SENSITIVE 654
#define RIPPA_SET_ESCAPE_CHARS 655
#define RIPPA_SET_ALWAYS_QUOTE 656
#define RIPPA_SET_REPEAT_MODES 657
#define RIPPA_SET_NEWLINE_STR 658
#define RIPPA_SET_DEFAULT_DIR 659
#define RIPPA_COMPRESS_FILE 660
#define RIPPA_DECOMPRESS_FILE 661
#define RIPPA_OPEN_DIALOG 662
#define RIPPA_ADD_REPEATED_ITEM 663
#define RIPPA_CLOSE_DIALOG 664
#define RIPPA_REFRESH_DIALOG 665
#define RIPPA_IO_CSV 666
#define RIPPA_IO_CSV_ARRAY 667
#define RIPPA_PUT_DATA_INTO_CHART 668
#define RIPPA_FIND_SUBGROUPS 669
#define RIPPA_SET_GROUP_OPTS 670
#define RIPPA_CHANGE_GLOBAL_KEY 671
#define RIPPA_REORDER_GROUP 672
#define RIPPA_SET_PART_IN_ITEM 673
#define RIPPA_GET_DATA_FROM_ARRAY 674
#define RIPPA_PUT_DATA_INTO_ARRAY 675
#define RIPPA_GET_DATA_FROM_CHART 676
#define RIPPA_GET_SETTINGS_FROM_CHART 677
#define RIPPA_PUT_SETTINGS_INTO_CHART 678
#define RIPPA_CHANGE_FILE 679
#define RIPPA_PERFORM_UNDO 680
#define RIPPA_PERFORM_REDO 681
#define RIPPA_CLEAR_STACKS 682
#define RIPPA_FORCE_ADD_UNDO_BLOCK 683
#define RIPPA_SET_MODE 684

#define RIPPC_CURRENT_GROUP_EXISTS 701
#define RIPPC_CURRENT_GROUP_HAS_ITEM 702
#define RIPPC_ITEM_HAS_HASH 703
#define RIPPC_GROUP_EXISTS 704
#define RIPPC_GROUP_HAS_ITEM 705
#define RIPPC_GROUP_ITEM_HAS_HASH 706
#define RIPPC_IS_WILDCAT_MATCH 707
#define RIPPC_ON_OPEN 708
#define RIPPC_ON_CLOSE 709
#define RIPPC_ON_MODIFIED 710
#define RIPPC_ON_DATA_MODIFIED 711
#define RIPPC_ON_SAVE 712
#define RIPPC_ON_NEW_ITEM 713
#define RIPPC_ON_RESULTS_FOUND 714
#define RIPPC_ON_NO_RESULTS_FOUND 715
#define RIPPC_ON_MERGE 716
#define RIPPC_ON_UNCOMPRESSION_OK 717
#define RIPPC_ON_UNCOMPRESSION_FAIL 718
#define RIPPC_HAS_UNDO 719
#define RIPPC_HAS_REDO 720

#define RIPPE_GROUP_GET_ITEM_VALUE 801
#define RIPPE_GROUP_GET_ITEM_STRING 802
#define RIPPE_GROUP_GET_POSITION_X 803
#define RIPPE_GROUP_GET_POSITION_Y 804
#define RIPPE_GROUP_GET_NTH_ITEM_NAME 805
#define RIPPE_GROUP_GET_NTH_ITEM_VALUE 806
#define RIPPE_GROUP_GET_NTH_ITEM_STRING 807
#define RIPPE_GROUP_GET_NUM_ITEMS 808
#define RIPPE_GET_ITEM_VALUE 809
#define RIPPE_GET_ITEM_STRING 810
#define RIPPE_GET_POSITION_X 811
#define RIPPE_GET_POSITION_Y 812
#define RIPPE_GET_NTH_GROUP 813
#define RIPPE_GET_NTH_ITEM_NAME 814
#define RIPPE_GET_NTH_ITEM_STRING 815
#define RIPPE_GET_NTH_ITEM_VALUE 816
#define RIPPE_GET_NUM_GROUPS 817
#define RIPPE_GET_NUM_ITEMS 818
#define RIPPE_GET_NUM_ITEMS_EVERYWHERE 819
#define RIPPE_GET_NUM_SEARCH_RESULTS 820
#define RIPPE_GET_NTH_SEARCH_RESULT_GROUP 821
#define RIPPE_GET_NTH_SEARCH_RESULT_ITEM_NAME 822
#define RIPPE_GET_NTH_SEARCH_RESULT_ITEM_VALUE 823
#define RIPPE_GET_NTH_SEARCH_RESULT_ITEM_STRING 824
#define RIPPE_GET_NTH_SEARCH_RESULT_PATH 825
#define RIPPE_GET_STRINGIFIED 826
#define RIPPE_HASH_STRING 827
#define RIPPE_CRYPT 828
#define RIPPE_ESCAPE_STRING 829
#define RIPPE_UNESCAPE_STRING 830
#define RIPPE_PERFORM_CALCULATIONS 831
#define RIPPE_FIND_MIN_OR_MAX_GROUP 832
#define RIPPE_GET_SORTED_NTH_ITEM_NAME 833
#define RIPPE_GET_SORTED_NTH_ITEM_VALUE 834
#define RIPPE_GET_CSVIFIED 835
#define RIPPE_GET_NTH_ITEM_EVERYWHERE 836
#define RIPPE_GET_NUM_UNIQUE_ITEM_NAMES 837
#define RIPPE_GET_PART_IN_ITEM 838
#define RIPPE_GROUP_GET_NAME 839
#define RIPPE_GROUP_GET_STRINGIFIED 840
#define RIPPE_GET_FILENAME 841

#define RIPPPR_VERSION_NAME 901
#define RIPPPR_VERSION_DESC 902
#define RIPPPR_GLOBAL_TICKBOX_NAME 903
#define RIPPPR_GLOBAL_TICKBOX_DESC 904
#define RIPPPR_GLOBAL_EDITBOX_NAME 905
#define RIPPPR_GLOBAL_EDITBOX_DESC 906
#define RIPPPR_FILEPATH_HEADER 907
#define RIPPPR_DEFAULT_FILE_NAME 908
#define RIPPPR_DEFAULT_FILE_DESC 909
#define RIPPPR_DEFAULT_FILE_FILTER 910
#define RIPPPR_READ_ONLY_NAME 911
#define RIPPPR_READ_ONLY_DESC 912
#define RIPPPR_BASE_DIR_NAME 913
#define RIPPPR_BASE_DIR_DESC 914
#define RIPPPR_BASE_DIR_WINDOWS 915
#define RIPPPR_BASE_DIR_CURRENT 916
#define RIPPPR_BASE_DIR_INSTALL 917
#define RIPPPR_BASE_DIR_APPDATA 918
#define RIPPPR_BASE_DIR_PRGDATA 919
#define RIPPPR_INITIAL_DATA_NAME 920
#define RIPPPR_INITIAL_DATA_DESC 921
#define RIPPPR_AUTO_LOAD_NAME 922
#define RIPPPR_AUTO_LOAD_DESC 923
#define RIPPPR_AUTO_SAVE_NAME 924
#define RIPPPR_AUTO_SAVE_DESC 925
#define RIPPPR_CREATE_DIRS_NAME 926
#define RIPPPR_CREATE_DIRS_DESC 927
#define RIPPPR_BASE1INDEX_NAME 928
#define RIPPPR_BASE1INDEX_DESC 929
#define RIPPPR_OBFUSCATION_HEADER 930
#define RIPPPR_COMPRESS_NAME 931
#define RIPPPR_COMPRESS_DESC 932
#define RIPPPR_ENCRYPT_NAME 933
#define RIPPPR_ENCRYPT_DESC 934
#define RIPPPR_UNDO_REDO_HEADER 935
#define RIPPPR_UNDO_NAME 936
#define RIPPPR_UNDO_DESC 937
#define RIPPPR_REDO_NAME 938
#define RIPPPR_REDO_DESC 939
#define RIPPPR_FORMAT_HEADER 940
#define RIPPPR_STANDARD_NAME 941
#define RIPPPR_STANDARD_DESC 942
#define RIPPPR_ADVANCED_NAME 943
#define RIPPPR_ADVANCED_DESC 944
#define RIPPPR_NEW_LINE_NAME 945
#define RIPPPR_NEW_LINE_DESC 946
#define RIPPPR_QUOTESTR_NAME 947
#define RIPPPR_QUOTESTR_DESC 948
#define RIPPPR_CASESENS_NAME 949
#define RIPPPR_CASESENS_DESC 950
#define RIPPPR_EMPTYGRP_NAME 951
#define RIPPPR_EMPTYGRP_DESC 952
#define RIPPPR_SUBGROUP_NAME 953
#define RIPPPR_SUBGROUP_DESC 954
#define RIPPPR_REPEATSG_NAME 955
#define RIPPPR_REPEATSG_DESC 956
#define RIPPPR_REPEATSI_NAME 957
#define RIPPPR_REPEATSI_DESC 958
#define RIPPPR_SAVERPTS_NAME 959
#define RIPPPR_SAVERPTS_DESC 960
#define RIPPPR_ESCAPESG_NAME 961
#define RIPPPR_ESCAPESG_DESC 962
#define RIPPPR_ESCAPESI_NAME 963
#define RIPPPR_ESCAPESI_DESC 964
#define RIPPPR_ESCAPESV_NAME 965
#define RIPPPR_ESCAPESV_DESC 966

#define RIPPH_TITLE 1001
#define RIPPH_PERFORM_CALCULATIONS 1002
#define RIPPH_FIND_MIN_OR_MAX_GROUP 1003

#define IDD_DEBUG 1101
#define IDD_OBJECTSELECT 1102
#define IDD_DEBUG2 1103
#define IDI_ICON1 1104
#define IDI_ICON2 1105
#define IDD_EDITITEM 1106
#define IDD_OPENDIALOG 1107
#define IDD_SLOTVIEW 1108
#define IDD_SSS 1109
#define IDD_SEARCH 1110
#define IDD_LOADMODE 1111
#define IDC_DATA 1112
#define IDC_EDIT 1113
#define IDC_LIST 1114
#define IDC_TREE 1115
#define IDC_NAME 1116
#define IDC_VALUE 1117
#define IDC_DELETE 1118
#define IDC_CHECK1 1119
#define IDC_CHECK2 1120
#define IDC_CHECK3 1121
#define IDC_CHECK4 1122
#define IDC_CHECK5 1123
#define IDC_CHECK6 1124
#define IDC_CHECK7 1125
#define IDC_CHECK8 1126
#define IDC_CHECK9 1127
#define IDC_CHECK10 1128
#define IDC_COMBO 1129
#define IDC_INFO 1130
#define IDC_LIST1 1131
#define IDC_RADIO 1132
#define IDC_RADIO2 1133
#define IDC_RADIO1 IDC_RADIO2 //yes, I don't know why, ask Jax
#define IDC_RADIO3 1134
#define IDC_RADIO4 1135
#define IDC_RADIO5 1136
#define IDC_RADIO6 1137
#define IDC_RADIO7 1138
#define IDC_RADIO8 1139
#define IDC_RADIO9 1140
#define IDC_RADIO10 1141
#define IDC_RADIO11 1142
#define IDC_DESCRIPTIONTEXT 1143
#define IDC_SORTINGRADIO1 1144
#define IDC_SORTINGRADIO2 1145
#define IDC_SORTINGRADIO3 1146
#define IDC_STATIC -1

#define RIPPCP_DIALOG_NO_DELETING_GROUPS 1201
#define RIPPCP_DIALOG_NO_DELETING_ITEMS 1202
#define RIPPCP_DIALOG_NO_RENAMING_GROUPS 1203
#define RIPPCP_DIALOG_NO_RENAMING_ITEMS 1204
#define RIPPCP_DIALOG_NO_CHANGING_VALUES 1205
#define RIPPCP_DIALOG_NO_REFRESH_BUTTON 1206
#define RIPPCP_DIALOG_NO_NEW_GROUPS 1207
#define RIPPCP_DIALOG_NO_NEW_ITEMS 1208
#define RIPPCP_DIALOG_NO_AUTO_REFRESH 1209
#define RIPPCP_DIALOG_PAUSE_APP 1210
#define RIPPCP_DIALOG_NONE 1211
#define RIPPCP_SSS_SORT_BY_VALUE 1212
#define RIPPCP_SSS_SHUFFLE 1213
#define RIPPCP_SSS_REMOVE_FIRST 1214
#define RIPPCP_SSS_CYCLE_UP 1215
#define RIPPCP_SSS_REMOVE_LAST 1216
#define RIPPCP_SSS_CYCLE_DOWN 1217
#define RIPPCP_SSS_REVERSE 1218
#define RIPPCP_SSS_SWAP_ITEMS_VALUES_IN 1219
#define RIPPCP_SSS_RENUMBER 1220
#define RIPPCP_SSS_SORT_BY_NAME 1221
#define RIPPCP_SSS_SORT_BY_FIRST_PART 1222
#define RIPPCP_SSS_SORT_ORDER_ALPHANUM 1223
#define RIPPCP_SSS_SORT_ORDER_STANDARD_CASE_INSENSITIVE 1224
#define RIPPCP_SSS_SORT_ORDER_STANDARD_CASE_SENSITIVE 1225
#define RIPPCP_SEARCH_MODE 1226
#define RIPPCP_SEARCH_MODE_DISREGARD 1227
#define RIPPCP_SEARCH_MODE_OR 1228
#define RIPPCP_SEARCH_MODE_AND 1229
#define RIPPCP_SEARCH_MODE_XOR 1230
#define RIPPCP_SEARCH_OPTIONS 1231
#define RIPPCP_SEARCH_OPTION_UNUSED 1232
#define RIPPCP_SEARCH_OPTION_ONLY_GROUP_NAMES 1233
#define RIPPCP_SEARCH_OPTION_MATCH_ANY 1234
#define RIPPCP_SEARCH_OPTION_CASE_SENSITIVE 1235
#define RIPPCP_SEARCH_OPTIONS_END 1236
#define RIPPCP_COMMA 1237
#define RIPPCP_NLC_LOAD 1238
#define RIPPCP_NLC_CHANGE 1239
#define RIPPCP_NLC_CHANGE_AND 1240
#define RIPPCP_NLC_KEEP_PATH_AND 1241
#define RIPPCP_NLC_CHANGE_IF_EXISTS_AND 1242
#define RIPPCP_NLC_CLEAR_DATA 1243
#define RIPPCP_NLC_KEEP_DATA 1244
#define RIPPCP_NLC_CLEAR_DATA_IF_FILE_EXISTS 1245
#define RIPPCP_NLC_SAVE_IMMEDIATELY 1246
#define RIPPCP_NLC_READ_ONLY 1247

#define RIPPCD_OBJECTSELECTOR1 1301
#define RIPPCD_OBJECTSELECTOR2 1302
#define RIPPCD_OBJECTSELECTOR3 1303
#define RIPPCD_OBJECTSELECTOR4 1304
#define RIPPCD_OBJECTSELECTOR5 1305
#define RIPPCD_DIALOG_ITEM_FORMATTING_A 1306
#define RIPPCD_DIALOG_ITEM_FORMATTING_B 1307
#define RIPPCD_DIALOG_ITEM_FORMATTING_C 1308
#define RIPPCD_DIALOG_ITEM_FORMATTING_D 1309
#define RIPPCD_DIALOG_ITEM_FORMATTING_E 1310
#define RIPPCD_DIALOG_ITEM_FORMATTING_F 1311
#define RIPPCD_DIALOG_ITEM_FORMATTING_G 1312
#define RIPPCD_DIALOG_ITEM_FORMATTING_H 1313
#define RIPPCD_SSS_A 1314
#define RIPPCD_SSS_B 1315
#define RIPPCD_SSS_C 1316
#define RIPPCD_SSS_D 1317
#define RIPPCD_SSS_E 1318
#define RIPPCD_SSS_F 1319
#define RIPPCD_SSS_G 1320
#define RIPPCD_SSS_H 1321
#define RIPPCD_SSS_I 1322
#define RIPPCD_SSS_J 1323
#define RIPPCD_SSS_K 1324

#endif //FUSION_RUNTIME_ONLY

#ifndef FUSION_RUNTIME_ONLY
#define EDIT_MODE
#endif

#endif