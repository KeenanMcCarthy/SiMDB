FROM alpine:3.17.0
WORKDIR /simDB
COPY . /simDB
RUN make -f Makefile
CMD ["./a.out", "SERVER"]
EXPOSE 3000
