function adaTalk()
comPort='COM4';
timeLimit=3600; %Time limit in seconds.
acqRate=1000;  %Acquisition rate in Hertz.

keepOn=false;
a=serial(comPort);
fopen(a);

stringInd=1;
startStopString={'Start', 'Stop'};

global outNums;
global accOutput;
outNums=uint16([]);

f=figure('CloseRequestFcn', @callbackCloseFig);
startStop=uicontrol('Parent', f,...
    'Style', 'pushbutton',...
    'String', startStopString{stringInd},...
    'Units', 'normalized',...
    'Position', [.4, .46, .2, .1],...
    'Callback', @callbackStartStop);

startStopSignal={'e','s'};
    function callbackStartStop(varargin)
        pauseTime=1/(10*acqRate);

        keepOn=~keepOn;
        stringInd=mod(stringInd,2)+1;

        set(startStop, 'String', startStopString{stringInd});
        if keepOn
            accOutput=uint8(zeros(30*timeLimit*acqRate,1)); %Pre-define
            runLoop=true;
            pause(.1);
            fwrite(a,'s');
        else
            fwrite(a,'e');
            runLoop=false;
        end
%         startStopSignal{stringInd}
%         fwrite(a,startStopSignal{stringInd});
%         byteLength=0;
        tic;
        startInd=1;
        while keepOn
            if toc>=timeLimit
                fwrite(a,'e');
                keepOn=false;
                disp(['Timed out after ' num2str(timeLimit) ' seconds.']);
                stringInd=mod(stringInd,2)+1;
                set(startStop, 'String', startStopString{stringInd});
                pause(.1);
            end
            byteLength=a.BytesAvailable;
            if byteLength
                newBytes=fread(a,byteLength);
                endInd=startInd+byteLength-1;
                accOutput(startInd:endInd)=newBytes;
%                 [startInd, endInd]
                startInd=endInd+1;
%                 disp(newBytes);
            end
            pause(pauseTime);
        end
        fwrite(a,'e');
        %Read any remaining bytes from the buffer
        if runLoop
            byteLength=a.BytesAvailable;
            while byteLength > 0
                newBytes=fread(a,byteLength);
                endInd=startInd+byteLength-1;
%                 [startInd,endInd]
                accOutput(startInd:endInd)=newBytes;
                startInd=endInd+1;
                pause(.05);
                byteLength=a.BytesAvailable;
            end

            keepOn=false;
            %Remove final semicolon (;=59 in ASCII) and everything after:
            firstInd=find(accOutput==59,1,'first')+1;
            firstCommas=find(accOutput==44,2,'first');
            if all(firstCommas < firstInd) && all(firstCommas > 1)
                firstInd=1;
            end
            lastInd=find(accOutput==59,1,'last')-1;
            accOutput=accOutput(firstInd:lastInd)';
            
            %Convert to table of numbers:
            outNums=eval(['uint16([' char(accOutput) ']);']);
%             outFile=uiputfile();
        end
    end

    function callbackCloseFig(varargin)
        try
            fclose(a);
        catch ME
            disp(ME);
        end
        delete(a);
        closereq;
    end

end





% fclose(a);


% keepOn=true;
% fun=@(a,b) eval('break');
% set(gcf, 'KeyPressFcn', @fun);
% k=1;
% while keepOn
%     if mod(k,1000)==1
%         disp(['Loop number ' num2str(k)]);
%     end
%     k=k+1;
%     pause(.01)
% end
% 
% 
% set(gcf,'KeyPressFcn','k=get(gcf,''currentchar'');');
% while 1
%   %do stuff here
%   pause(.01)
%   if ~isempty(k)
%     if strcmp(k,'s'); break; end;
%     if strcmp(k,'p'); pause; k=[]; end;
%   end
% end