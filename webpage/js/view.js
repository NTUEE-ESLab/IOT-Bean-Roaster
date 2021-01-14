// $("#my_nanogallery2").nanogallery2({
//     // ### gallery settings ###
//     thumbnailHeight:  150,
//     thumbnailWidth:   150,
//     itemsBaseURL:     'https://nanogallery2.nanostudio.org/samples/',

//     // ### gallery content ###
//     items: [
//         { src: 'img_01.jpg', srct: 'img_01t.jpg', title: 'Title Image 1' },
//         { src: 'img_02.jpg', srct: 'img_02t.jpg', title: 'Title Image 2' },
//         { src: 'img_03.jpg', srct: 'img_03t.jpg', title: 'Title Image 3' }
//     ]
//   });



$(document).ready(function () {
    fetch('/getAllData').then(function (response) {
        return response.json();
    })
        .then(function (myJson) {
            let lst_obj = []
            myJson['Data'].forEach(element => {
                lst_obj.push({ src: 'coffee.jpg', srct: 'coffee.jpg', title: element['label'], description: String(element['idtbl_coffee']) })
            });
            return lst_obj;

        }).then(function (mylst_obj) {
            $("#my_nanogallery2").nanogallery2({
                items: mylst_obj,
                thumbnailWidth: 'auto',
                thumbnailHeight: 200,
                itemsBaseURL: './img/',
                thumbnailLabel: {
                    position: "overImageOnBottom"
                },
                fnThumbnailOpen: mycallback,
                thumbnailHoverEffect2: "imageScaleIn80|imageSepiaOff|labelAppear75",
                thumbnailAlignment: "center",
                thumbnailOpenImage: true,

                locationHash: false
            });
            // console.log(asd);

        });

    // var ngy2data = $("#my_nanogallery2").nanogallery2('data');
    // var instance = $("#my_nanogallery2").nanogallery2('instance');
    // var ID = ngy2data.items.length + 1;
    // var albumID = '0';
    // var newItem = NGY2Item.New(instance, 'new berlin ' + ID, '', ID, albumID, 'image', '' );
    // newItem.thumbSet( 'coffee.jpg'); // w,h
    // define URL of the media to display in lightbox
    // newItem.setMediaURL( 'coffee.jpg', 'img');
    // newItem.addToGOM();
    // console.log(ngy2data)
    // console.log(instance)
    // $("#my_nanogallery2").on('itemSelected.nanogallery2', mycallback)
    function mycallback(items) {
        let go_to_url = `/getGraph.html?id=${items[0]['description']}`;
        // alert(got_to_url);
        window.location.assign(go_to_url);
        // window.open(go_to_url)
        console.log(items);
    }
});