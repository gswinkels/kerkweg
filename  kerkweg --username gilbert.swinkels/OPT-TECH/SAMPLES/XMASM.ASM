; XMASM.ASM - Assembly Language sample program that calls Opt-Tech Sort
;
STACK   SEGMENT STACK
        DB      50 DUP('STACKSEG')
STACK   ENDS
;
DATA    SEGMENT WORD

;PARAMETER BLOCK PASSED TO OPT-TECH SORT
PARMBLK LABEL   BYTE
INFILE_OFF      DW      ?               ;OFFSET  OF INPUT FILE NAME
INFILE_SEG      DW      ?               ;SEGMENT OF INPUT FILE NAME
INFILE_LEN      DW      ?               ;LENGTH  OF INPUT FILE NAME
OUTFILE_OFF     DW      ?               ;OFFSET  OF OUTPUT FILE NAME
OUTFILE_SEG     DW      ?               ;SEGMENT OF OUTPUT FILE NAME
OUTFILE_LEN     DW      ?               ;LENGTH  OF OUTPUT FILE NAME
CTLSTMT_OFF     DW      ?               ;OFFSET  OF CONTROL STATEMENT
CTLSTMT_SEG     DW      ?               ;SEGMENT OF CONTROL STATEMEMT
CTLSTMT_LEN     DW      ?               ;LENGTH  OF CONTROL STATEMENT
NUMREC_OFF      DW      ?               ;OFFSET  OF NUMBER OF RECS OUTPUT
NUMREC_SEG      DW      ?               ;SEGMENT OF NUMBER OF RECS OUTPUT
RETCODE_OFF     DW      ?               ;OFFSET  OF RETURN CODE
RETCODE_SEG     DW      ?               ;SEGMENT OF RETURN CODE

;HOLDING AREAS FOR PARAMETERS
INFILE_NAME      DB     'SAMP.DAT'
OUTFILE_NAME     DB     'SORTED.DAT'
CTLSTMT          DB     'SORT(1,10,C,A)'
NUMREC           DW     ?
RETCODE          DW     ?

MESSAGE1         DB     'CALLING OPT-TECH SORT...',0AH,0DH,'$'
MESSAGE2         DB     'BACK FROM OPT-TECH SORT',0AH,0DH,'$'
DATA    ENDS

        EXTRN   _MAINS:FAR      ;FAR ENTRY POINT TO OPT-TECH SORT

CODE    SEGMENT PUBLIC  'CODE'
ASSUME  CS:CODE, DS:DATA
XMASM           PROC    FAR
;SET-UP FOR RETURN TO OPERATING SYSTEM
        PUSH    DS                      ;PUSH RETURN SEGMENT
        XOR     AX,AX                   ;CLEAR AX
        PUSH    AX                      ;PUSH RETURN OFFSET

;SET-UP DATA SEGMENT ADDRESSABILITY
        MOV     AX,DATA                 ;GET SEGMENT ADDRESS OF DATA
        MOV     DS,AX                   ;SET DS TO POINT TO DATA SEGMENT

;RELEASE UNUSED MEMORY TO DOS SO THAT OPT-TECH SORT WILL BE ABLE TO 
;DYNAMICALLY ALLOCATE.  A FIXED SIZE AREA CAN ALSO BE LINKED IN E.G. SW40K
        MOV     BX,8192                 ;ALLOCATE A FIXED 128K FOR THIS PGM
        MOV     AH,4AH                  ;SET 'SETBLOCK' FUNCTION, ES IS OK
        INT     21H                     ;CALL DOS TO DO SETBLOCK
        JC      LEAVE                   ;IF ERROR, JUST LEAVE

;SET-UP PARAMETER BLOCK FOR CALL TO SORT
        MOV     INFILE_OFF,OFFSET INFILE_NAME
        MOV     INFILE_SEG,SEG INFILE_NAME
        MOV     INFILE_LEN,8

        MOV     OUTFILE_OFF,OFFSET OUTFILE_NAME
        MOV     OUTFILE_SEG,SEG OUTFILE_NAME
        MOV     OUTFILE_LEN,10

        MOV     CTLSTMT_OFF,OFFSET CTLSTMT
        MOV     CTLSTMT_SEG,SEG CTLSTMT
        MOV     CTLSTMT_LEN,14

        MOV     NUMREC_OFF, OFFSET NUMREC
        MOV     NUMREC_SEG, SEG NUMREC
        
        MOV     RETCODE_OFF, OFFSET RETCODE
        MOV     RETCODE_SEG, SEG RETCODE

        MOV     DX,OFFSET MESSAGE1      ;PRINT MESSAGE - CALLING SORT
        MOV     AH,9
        INT     21H
        
;SET DS:DX TO POINT TO PARAMETER BLOCK
        MOV     DX, OFFSET PARMBLK
        CALL    _MAINS
; NOTE: A CALL TO THE RESIDENT SORT (SORTRES) COULD ALSO BE DONE HERE
; BY CALLING SOFTWARE INTERRUPT 7DH

;RETCODE WOULD NORMALLY BE CHECKED HERE

        MOV     DX,OFFSET MESSAGE2      ;PRINT MESSAGE - BACK FROM SORT
        MOV     AH,9
        INT     21H
LEAVE:  
        RET                             ;RETURN TO DOS
XMASM   ENDP
CODE    ENDS
        END     XMASM
