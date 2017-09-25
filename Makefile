default: test

IMAGES_REPO ?=
IMAGE_NAME ?= kunbound
IMAGE_TAG ?= latest
TEST_HOST ?= google.com
HELM_RELEASE ?= kunbound
KUBE_CONTEXT ?= $(shell kubectl config current-context)
DRY_RUN := --debug --dry-run
NO_CACHE :=

.PHONY: no-cache
no-cache:
	$(eval NO_CACHE := --no-cache)
	@:

.PHONY: image
image:
	docker build $(NO_CACHE) --rm=true --force-rm=true --tag=$(IMAGE_NAME):$(IMAGE_TAG) .

.PHONY: test
test: image
	docker run -d --name $(IMAGE_NAME)-test -p 8053:53/udp $(IMAGE_NAME):$(IMAGE_TAG)
	dig @localhost -p 8053 $(TEST_HOST)
	docker rm -f $(IMAGE_NAME)-test

.PHONY: push
push: test
	@if [ -z "$${IMAGES_REPO}" ]; then \
		echo "Error: set IMAGES_REPO to the docker hub repository to push to" && exit 1; \
	fi
	docker tag $(IMAGE_NAME):$(IMAGE_TAG) $(IMAGES_REPO)/$(IMAGE_NAME):$(IMAGE_TAG)
	docker push $(IMAGES_REPO)/$(IMAGE_NAME):$(IMAGE_TAG)

.PHONY: apply
apply:
	$(eval DRY_RUN := )
	@:

.PHONY: release
release: push
	cd unbound && \
	helm upgrade --install $(HELM_RELEASE) . --kube-context $(KUBE_CONTEXT) --set image=$(IMAGES_REPO)/$(IMAGE_NAME):$(IMAGE_TAG) $(DRY_RUN); \
	cd ..
