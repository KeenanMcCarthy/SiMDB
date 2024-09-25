FROM alpine:3.17.0
WORKDIR /simDB
COPY . /simDB
RUN make -f Makefile
CMD ["./SiMDB", "SERVER"]
EXPOSE 3000
