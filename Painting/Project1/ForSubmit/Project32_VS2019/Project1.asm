;;exe로 바꾸고 파일명을 "mypaint"로 바꾸기



include irvine32.inc

distr proto  sl:byte, endpoint:byte
dfs proto loc:word, oric:byte, dfscolor:byte
paint proto locat:word, paintcolor:byte

.data
input byte 129 dup(0)              ;input값
source byte 20 dup(0)                  ;초기값이 0이니까 "~~",0 이되지   ;input의 source img file
output byte 20 dup(0)                                        ;input의 output img file
xval byte 2 dup(0)                                                                      ;input의 x값->채우기시작x값
yval byte 2 dup(0)                        ;;3100 if y가 1이면                                                       ;input의 y값->채우기시작y값
color byte 	0;input의 color 


filehandle dword ?

firstrow byte 7 dup(0) ; 첫 줄
frbufsize dword ($-firstrow)

mapsbuffer byte 50 dup(52 dup(0)) ;50*(50+2)=2607 ->줄 마지막에 0dh,0ah
mapbufsize dword ($-mapsbuffer)
bytecount dword ?
byteswritten dword ?

intx byte ?
inty byte ?

xyloc word 0 ;xy처음위치를 maps에 맞게변환해서 
xyoric byte 0 ; 맨 처음 색->"문자"임(숫자가 아니라)

visitedmap byte 50 dup(52 dup(0)) ;;;;;;;방문했으면 0->1

.code

main proc
	mov edx, offset input  ;;input값 분산완료될 때까지 절대 건들면 안됨
	call getcommandtail
	add edx,1;;;;;;;;;앞의 필요없는부분 빼고
	;;;input값을 요소별로 분산시켜 저장함
	mov ebx,offset source
	invoke distr, lengthof source, 20h;;인출할거 최대크기(반복횟수), 중단점, 여기에  ecx가 들어가야지 이 두 register는 보존 안함
	mov ebx, offset output
	invoke distr, lengthof output, 20h
	mov ebx, offset xval
	invoke distr, lengthof xval, 20h
	mov ebx, offset yval
	invoke distr, lengthof yval, 20h
	mov ebx, offset color
	invoke distr, lengthof color, 0h

	

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;다 distribute했으니 파일 불러와야지->source,output은 변환시킬필요 없나? 
	
	invoke createfile,
			addr source, generic_read,
			do_not_share, null, open_existing,
			file_attribute_normal,0
	mov filehandle, eax
	invoke readfile,
			filehandle, addr firstrow,
			frbufsize, addr bytecount,0
	invoke readfile,
			filehandle, addr mapsbuffer,
			mapbufsize, addr bytecount, 0
	invoke closehandle,
			filehandle



	


	;;;;;;xval,yval,color은 아스키코드로->문자or숫자로 변환해야함 color은 0~7 x,y는 0~49  
	;;30h~39h가 0~9임
	;;xval,yval은 두번째자리가 0h면 한자리수, 아니면 두자리수('01'로 입력해도 상관없징)
	;;;;;;;;;;xval대해
	mov al, 0h
	cmp al, xval[1]
	je xones
	jmp xtens
xones:
	mov al, 30h
	sub xval[0], al ;;;이러면 0~9가 나오징
	mov al, xval[0]
	mov intx,al
	jmp abouty
xtens:
	mov al,30h
	sub xval[1],al
	sub xval[0],al
	mov al, xval[0]
	mov ah,0
	movzx bx,xval[1]
	mov ecx,10
	mul ecx
	add ax,bx
	mov intx, al
	mov al, intx
	jmp abouty

abouty:
;;;;;;;;;;;;;;;;;;yval대해
	mov al, 0h
	cmp al, yval[1]
	je yones
	jmp ytens
yones:
	mov al, 30h
	sub yval[0], al ;;;이러면 0~9가 나오징
	mov al, yval[0]
	mov inty,al
	jmp aboutmaps
ytens:
	mov al,30h
	sub yval[1],al
	sub yval[0],al
	mov al, yval[0]
	mov ah,0
	movzx bx,yval[1]
	mov ecx,10
	mul ecx
	add ax,bx
	mov inty, al
	jmp aboutmaps


aboutmaps:
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 이제 (x,y)로가서 값 바꾸고 같은 영역 인지해야함 
	;;mapsbuffer는 한줄에 52개, 마지가 두개(0dh,0ah 순서이대로임)는 줄바꾸기용 기억=>위:-=52, 아래:+=52, 왼:-=1, 오:+=1
	;;(x,y)는 mapsbuffer에서 x+(52*y)위치임  단위는 word로 고려
	mov ax,0
	mov al, inty 
	mov bl,52
	mul bl
	movzx bx,intx
	add ax,bx
	mov xyloc,ax
	
	;;;;;;;;;;;;xyloc구했음 
	movzx eax,xyloc
	mov bl, mapsbuffer[eax]
	mov xyoric, bl
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;xyoric구했음 ->아스키코드 즉, 문자임
	


	mov ebx, offset mapsbuffer ;;;;;;;;;;;;;;;색칠하는동안 ebx건들지 말기
	mov edx, offset visitedmap ;;;;;;;;;;;;;;;색칠하는동안 edx건들지 말기
	
	;;(29,8)이면 29+52*8=445
	

	invoke dfs, xyloc, xyoric, color
	







	invoke createfile,
			addr output, generic_write,
			do_not_share, null, create_always, 
			file_attribute_normal, 0
	mov filehandle, eax
	invoke writefile,
			filehandle, addr firstrow,
			frbufsize, addr byteswritten,0
	invoke writefile,
			filehandle, addr mapsbuffer,
			mapbufsize, addr byteswritten,0
	invoke closehandle,
			filehandle

	exit
main endp


;; loop통해 중단점기준으로 나눠서 필요한 인수들 저장시키기 
;;sl은 반복가능횟수 max, endpoint는 중단점
distr proc sl:byte, endpoint:byte ;;edx는 input의 주소, ebx는 source의 주소 
	push ebp
	mov esi,ebp
	push ecx
	movzx ecx, sl ;;일단 반복가능횟수max
dL1:	
	mov al, [edx] ;;source에 input element채우기
	mov [ebx],al

	inc edx
	inc ebx
	mov al, endpoint ;;다음 element가 중단점인지 체크
	cmp [edx], al
	je quit
	loop dL1

quit:
	inc edx
	pop ecx
	pop ebp
	ret 8
distr endp

;;;;;;;;;;;;;;;;;;;;depth first search방법으로 인접한 같은 영역에 색칠하기
;;;;;loc은 현재 위치, oric은 input으로 들어온 위치의 색, dfscolor은 색칠할 색, 
dfs proc loc:word, oric:byte, dfscolor:byte ; ebx는 map, edx는 visited
	local wayr:word, wayl:word, wayu:word, wayd :word ;;;사방중 하나로 이동한 위치
	;;;;;;;;;;;일단 지역변수로 xyloc의 사방을 넣고 보는겨-> 위의 local 변수들은 (사방중 하나로)이동한 위치
	mov ax, loc
	mov wayr, ax
	inc wayr
	mov wayl,ax
	dec wayl
	mov wayu,ax
	sub wayu,52
	mov wayd,ax
	add wayd,52


	invoke paint, loc, dfscolor  ;;ebx는 map, edx는 visited  =>색칠하기 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;loc가 이상하게 높게 나오는데
	
	;;오른쪽부터 시계반대방향 순으로 => xyloc에서: +1-> -52 -> -1 -> +52
	;;경계나 다른영역은 아닌지 체크 그리고 방문여부도 체크
probr:
	mov ax,0
	cmp wayr, ax   ;;locarr[ecx]는 사방중 하나로 이동한 위치
	jl probu
	;;;통과->맨 윗줄은 아니지
	mov ax,2600
	cmp wayr,ax
	jg probu
	;;;통과->맨 아랫줄은 아니지
	movzx eax,wayr
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probu
	;;;통과->다른영역은 아니지
	movzx eax,wayr
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probu
	;;;통과->첫 방문이지
	invoke dfs, wayr, oric, dfscolor
probu:
	mov ax,0
	cmp wayu, ax   ;;locarr[ecx]는 사방중 하나로 이동한 위치
	jl probl
	;;;통과->맨 윗줄은 아니지
	mov ax,2600
	cmp wayu,ax
	jg probl
	;;;통과->맨 아랫줄은 아니지
	movzx eax,wayu
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probl
	;;;통과->다른영역은 아니지
	movzx eax,wayu
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probl
	;;;통과->첫 방문이지
	invoke dfs, wayu, oric, dfscolor
probl:
	mov ax,0
	cmp wayl, ax   ;;locarr[ecx]는 사방중 하나로 이동한 위치
	jl probd
	;;;통과->맨 윗줄은 아니지
	mov ax,2600
	cmp wayl,ax
	jg probd
	;;;통과->맨 아랫줄은 아니지
	movzx eax,wayl
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probd
	;;;통과->다른영역은 아니지
	movzx eax,wayl
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probd
	;;;통과->첫 방문이지
	invoke dfs, wayl, oric, dfscolor
probd:
	mov ax,0
	cmp wayd, ax   ;;locarr[ecx]는 사방중 하나로 이동한 위치
	jl dfsret
	;;;통과->맨 윗줄은 아니지
	mov ax,2600
	cmp wayd,ax
	jg dfsret
	;;;통과->맨 아랫줄은 아니지
	movzx eax,wayd
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne dfsret
	;;;통과->다른영역은 아니지
	movzx eax,wayd
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne dfsret
	;;;통과->첫 방문이지
	invoke dfs, wayd, oric, dfscolor

dfsret:
	ret
dfs endp

;;;색칠하기 locat은 색칠할 위치, dfs는 색칠할 색
paint proc locat:word, dfscolor:byte ;;ebx는 map, edx는 visited
	movzx eax,locat;
	add eax,edx
	mov cl,1
	mov [eax],cl ;;방문함

	movzx eax,locat
	add eax,ebx
	mov cl,dfscolor
	mov [eax],cl ;;색칠함

	ret
paint endp

end main