;;exe�� �ٲٰ� ���ϸ��� "mypaint"�� �ٲٱ�



include irvine32.inc

distr proto  sl:byte, endpoint:byte
dfs proto loc:word, oric:byte, dfscolor:byte
paint proto locat:word, paintcolor:byte

.data
input byte 129 dup(0)              ;input��
source byte 20 dup(0)                  ;�ʱⰪ�� 0�̴ϱ� "~~",0 �̵���   ;input�� source img file
output byte 20 dup(0)                                        ;input�� output img file
xval byte 2 dup(0)                                                                      ;input�� x��->ä������x��
yval byte 2 dup(0)                        ;;3100 if y�� 1�̸�                                                       ;input�� y��->ä������y��
color byte 	0;input�� color 


filehandle dword ?

firstrow byte 7 dup(0) ; ù ��
frbufsize dword ($-firstrow)

mapsbuffer byte 50 dup(52 dup(0)) ;50*(50+2)=2607 ->�� �������� 0dh,0ah
mapbufsize dword ($-mapsbuffer)
bytecount dword ?
byteswritten dword ?

intx byte ?
inty byte ?

xyloc word 0 ;xyó����ġ�� maps�� �°Ժ�ȯ�ؼ� 
xyoric byte 0 ; �� ó�� ��->"����"��(���ڰ� �ƴ϶�)

visitedmap byte 50 dup(52 dup(0)) ;;;;;;;�湮������ 0->1

.code

main proc
	mov edx, offset input  ;;input�� �л�Ϸ�� ������ ���� �ǵ�� �ȵ�
	call getcommandtail
	add edx,1;;;;;;;;;���� �ʿ���ºκ� ����
	;;;input���� ��Һ��� �л���� ������
	mov ebx,offset source
	invoke distr, lengthof source, 20h;;�����Ұ� �ִ�ũ��(�ݺ�Ƚ��), �ߴ���, ���⿡  ecx�� ������ �� �� register�� ���� ����
	mov ebx, offset output
	invoke distr, lengthof output, 20h
	mov ebx, offset xval
	invoke distr, lengthof xval, 20h
	mov ebx, offset yval
	invoke distr, lengthof yval, 20h
	mov ebx, offset color
	invoke distr, lengthof color, 0h

	

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;�� distribute������ ���� �ҷ��;���->source,output�� ��ȯ��ų�ʿ� ����? 
	
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



	


	;;;;;;xval,yval,color�� �ƽ�Ű�ڵ��->����or���ڷ� ��ȯ�ؾ��� color�� 0~7 x,y�� 0~49  
	;;30h~39h�� 0~9��
	;;xval,yval�� �ι�°�ڸ��� 0h�� ���ڸ���, �ƴϸ� ���ڸ���('01'�� �Է��ص� �����¡)
	;;;;;;;;;;xval����
	mov al, 0h
	cmp al, xval[1]
	je xones
	jmp xtens
xones:
	mov al, 30h
	sub xval[0], al ;;;�̷��� 0~9�� ����¡
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
;;;;;;;;;;;;;;;;;;yval����
	mov al, 0h
	cmp al, yval[1]
	je yones
	jmp ytens
yones:
	mov al, 30h
	sub yval[0], al ;;;�̷��� 0~9�� ����¡
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
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; ���� (x,y)�ΰ��� �� �ٲٰ� ���� ���� �����ؾ��� 
	;;mapsbuffer�� ���ٿ� 52��, ������ �ΰ�(0dh,0ah �����̴����)�� �ٹٲٱ�� ���=>��:-=52, �Ʒ�:+=52, ��:-=1, ��:+=1
	;;(x,y)�� mapsbuffer���� x+(52*y)��ġ��  ������ word�� ���
	mov ax,0
	mov al, inty 
	mov bl,52
	mul bl
	movzx bx,intx
	add ax,bx
	mov xyloc,ax
	
	;;;;;;;;;;;;xyloc������ 
	movzx eax,xyloc
	mov bl, mapsbuffer[eax]
	mov xyoric, bl
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;xyoric������ ->�ƽ�Ű�ڵ� ��, ������
	


	mov ebx, offset mapsbuffer ;;;;;;;;;;;;;;;��ĥ�ϴµ��� ebx�ǵ��� ����
	mov edx, offset visitedmap ;;;;;;;;;;;;;;;��ĥ�ϴµ��� edx�ǵ��� ����
	
	;;(29,8)�̸� 29+52*8=445
	

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


;; loop���� �ߴ����������� ������ �ʿ��� �μ��� �����Ű�� 
;;sl�� �ݺ�����Ƚ�� max, endpoint�� �ߴ���
distr proc sl:byte, endpoint:byte ;;edx�� input�� �ּ�, ebx�� source�� �ּ� 
	push ebp
	mov esi,ebp
	push ecx
	movzx ecx, sl ;;�ϴ� �ݺ�����Ƚ��max
dL1:	
	mov al, [edx] ;;source�� input elementä���
	mov [ebx],al

	inc edx
	inc ebx
	mov al, endpoint ;;���� element�� �ߴ������� üũ
	cmp [edx], al
	je quit
	loop dL1

quit:
	inc edx
	pop ecx
	pop ebp
	ret 8
distr endp

;;;;;;;;;;;;;;;;;;;;depth first search������� ������ ���� ������ ��ĥ�ϱ�
;;;;;loc�� ���� ��ġ, oric�� input���� ���� ��ġ�� ��, dfscolor�� ��ĥ�� ��, 
dfs proc loc:word, oric:byte, dfscolor:byte ; ebx�� map, edx�� visited
	local wayr:word, wayl:word, wayu:word, wayd :word ;;;����� �ϳ��� �̵��� ��ġ
	;;;;;;;;;;;�ϴ� ���������� xyloc�� ����� �ְ� ���°�-> ���� local �������� (����� �ϳ���)�̵��� ��ġ
	mov ax, loc
	mov wayr, ax
	inc wayr
	mov wayl,ax
	dec wayl
	mov wayu,ax
	sub wayu,52
	mov wayd,ax
	add wayd,52


	invoke paint, loc, dfscolor  ;;ebx�� map, edx�� visited  =>��ĥ�ϱ� ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;loc�� �̻��ϰ� ���� �����µ�
	
	;;�����ʺ��� �ð�ݴ���� ������ => xyloc����: +1-> -52 -> -1 -> +52
	;;��質 �ٸ������� �ƴ��� üũ �׸��� �湮���ε� üũ
probr:
	mov ax,0
	cmp wayr, ax   ;;locarr[ecx]�� ����� �ϳ��� �̵��� ��ġ
	jl probu
	;;;���->�� ������ �ƴ���
	mov ax,2600
	cmp wayr,ax
	jg probu
	;;;���->�� �Ʒ����� �ƴ���
	movzx eax,wayr
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probu
	;;;���->�ٸ������� �ƴ���
	movzx eax,wayr
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probu
	;;;���->ù �湮����
	invoke dfs, wayr, oric, dfscolor
probu:
	mov ax,0
	cmp wayu, ax   ;;locarr[ecx]�� ����� �ϳ��� �̵��� ��ġ
	jl probl
	;;;���->�� ������ �ƴ���
	mov ax,2600
	cmp wayu,ax
	jg probl
	;;;���->�� �Ʒ����� �ƴ���
	movzx eax,wayu
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probl
	;;;���->�ٸ������� �ƴ���
	movzx eax,wayu
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probl
	;;;���->ù �湮����
	invoke dfs, wayu, oric, dfscolor
probl:
	mov ax,0
	cmp wayl, ax   ;;locarr[ecx]�� ����� �ϳ��� �̵��� ��ġ
	jl probd
	;;;���->�� ������ �ƴ���
	mov ax,2600
	cmp wayl,ax
	jg probd
	;;;���->�� �Ʒ����� �ƴ���
	movzx eax,wayl
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne probd
	;;;���->�ٸ������� �ƴ���
	movzx eax,wayl
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne probd
	;;;���->ù �湮����
	invoke dfs, wayl, oric, dfscolor
probd:
	mov ax,0
	cmp wayd, ax   ;;locarr[ecx]�� ����� �ϳ��� �̵��� ��ġ
	jl dfsret
	;;;���->�� ������ �ƴ���
	mov ax,2600
	cmp wayd,ax
	jg dfsret
	;;;���->�� �Ʒ����� �ƴ���
	movzx eax,wayd
	mov cl, oric
	add eax,ebx
	cmp [eax],cl
	jne dfsret
	;;;���->�ٸ������� �ƴ���
	movzx eax,wayd
	add eax,edx
	mov cl,0
	cmp [eax],cl
	jne dfsret
	;;;���->ù �湮����
	invoke dfs, wayd, oric, dfscolor

dfsret:
	ret
dfs endp

;;;��ĥ�ϱ� locat�� ��ĥ�� ��ġ, dfs�� ��ĥ�� ��
paint proc locat:word, dfscolor:byte ;;ebx�� map, edx�� visited
	movzx eax,locat;
	add eax,edx
	mov cl,1
	mov [eax],cl ;;�湮��

	movzx eax,locat
	add eax,ebx
	mov cl,dfscolor
	mov [eax],cl ;;��ĥ��

	ret
paint endp

end main