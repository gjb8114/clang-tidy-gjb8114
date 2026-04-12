# Stage 0: Shared base with LLVM apt repository configured
FROM ubuntu:24.04 AS llvm-base

ARG LLVM_VERSION=18

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    ca-certificates \
    gnupg \
    lsb-release \
    wget \
    && rm -rf /var/lib/apt/lists/*

# Add the official LLVM apt repository using the recommended binary GPG format
RUN wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key \
    | gpg --dearmor -o /etc/apt/trusted.gpg.d/apt.llvm.org.gpg \
    && echo "deb http://apt.llvm.org/$(lsb_release -cs)/ llvm-toolchain-$(lsb_release -cs)-${LLVM_VERSION} main" \
       > /etc/apt/sources.list.d/llvm.list

# Stage 1: Build
FROM llvm-base AS builder

ARG LLVM_VERSION=18
ARG BUILD_TYPE=Release

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-${LLVM_VERSION} \
    clang-tidy-${LLVM_VERSION} \
    cmake \
    git \
    libclang-${LLVM_VERSION}-dev \
    llvm-${LLVM_VERSION}-dev \
    make \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN cmake \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DClang_DIR=/usr/lib/llvm-${LLVM_VERSION}/lib/cmake/clang \
    -DCMAKE_PROGRAM_PATH=/usr/lib/llvm-${LLVM_VERSION}/bin \
    -S . -B build \
    && cmake --build build --target clang-tidy-gjb8114 -- -j"$(nproc)"

# Stage 2: Runtime image
FROM llvm-base AS runtime

ARG LLVM_VERSION=18

RUN apt-get update && apt-get install -y --no-install-recommends \
    clang-tidy-${LLVM_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Create a convenience symlink so users can call `clang-tidy` directly
RUN ln -sf /usr/bin/clang-tidy-${LLVM_VERSION} /usr/local/bin/clang-tidy

COPY --from=builder /src/build/lib/libclang-tidy-gjb8114.so /usr/local/lib/

WORKDIR /workspace

ENTRYPOINT ["clang-tidy"]
CMD ["--help"]
